// Copyright © 2026 CCP ehf.
//
// Integration-test entry point.
//
// Unlike TrinityEngineTest (a bare unit harness whose main() just calls RUN_ALL_TESTS), this runs
// the gtest cases INSIDE a Stackless tasklet, matching how the engine runs in the client. Engine
// code that requires a *current tasklet* — e.g. ScopedBlockTrap in ExecuteMainThreadActions — works
// here, instead of dereferencing a null current-tasklet (the 0x...C0 access violation we hit in the
// bare harness).
//
// The interpreter is NOT initialized in this process by default — confirmed: blue.dll loads
// python27.dll but nothing calls Py_Initialize here (Py_IsInitialized()==0 when launched standalone).
// So main() brings Python up itself: it points PYTHONHOME at this exe's directory, where CMake deploys
// a py2.7 stdlib as Lib/ (copied from carbon/common/stdlib), then runs the gtest cases inside a tasklet.
//
// NOTE: this is the minimal bootstrap (bare Py_Initialize against a filesystem stdlib, à la the old
// stackless testbench). It deliberately does NOT do the full Blue init (BlueModuleStartup + manifest
// paths); if a test ever needs Blue's resman/exposure machinery, that bootstrap would have to be added.

#include "StdAfx.h"        // pulls in <Python.h>
#include "gtest/gtest.h"

#include <cstring>         // strncmp

const char* g_moduleName = "TrinityEngineIntegrationTest";

namespace
{
int g_testResult = 1;

// Invoked from within the Stackless tasklet created by the bootstrap snippet below.
PyObject* RunAllTestsInTasklet( PyObject*, PyObject* )
{
	g_testResult = RUN_ALL_TESTS();
	Py_RETURN_NONE;
}

PyMethodDef s_methods[] = {
	{ "_run_all_tests", RunAllTestsInTasklet, METH_NOARGS, "Run the gtest suite inside a Stackless tasklet." },
	{ nullptr, nullptr, 0, nullptr },
};
}

int main( int argc, char** argv )
{
	// gtest_discover_tests enumerates by running us with --gtest_list_tests. Listing runs no test
	// bodies, so it needs no tasklet — and routing it through the Stackless bootstrap below breaks
	// discovery (the integration target's tests then never appear in Test Explorer). Detect the flag
	// before InitGoogleTest strips it from argv, and take the plain listing path for discovery.
	bool listTestsOnly = false;
	for( int i = 1; i < argc; ++i )
	{
		if( strncmp( argv[i], "--gtest_list_tests", 18 ) == 0 )
		{
			listTestsOnly = true;
			break;
		}
	}

	CCP::SetLogMainThreadId();
	::testing::InitGoogleTest( &argc, argv );

	if( listTestsOnly )
	{
		return RUN_ALL_TESTS();   // lists test names to stdout, runs no bodies — the discovery path
	}

	if( !Py_IsInitialized() )
	{
		// Bring up the interpreter. Point PYTHONHOME at this exe's directory, where CMake deployed the
		// py2.7 stdlib as Lib/ — so getpath finds <home>/Lib/os.py and builds sys.path from it.
		// Py_SetPythonHome keeps the pointer, so the buffer must outlive the process -> static.
		static char s_pythonHome[MAX_PATH] = { 0 };
		GetModuleFileNameA( nullptr, s_pythonHome, MAX_PATH );
		if( char* slash = strrchr( s_pythonHome, '\\' ) )
		{
			*slash = '\0';   // strip the exe filename -> exe directory
		}
		Py_SetPythonHome( s_pythonHome );
		Py_NoSiteFlag = 1;        // skip site.py / site-packages during init (minimise what the stdlib must provide)
		PyEval_InitThreads();
		Py_Initialize();          // fatals (aborts) if it can't import `encodings` from <home>/Lib
		fprintf( stderr, "[integration] Py_Initialize: Py_IsInitialized()=%d  home=%s\n", Py_IsInitialized(), s_pythonHome );
	}

	if( !Py_IsInitialized() )
	{
		fprintf( stderr, "[integration] Py_Initialize failed — check the deployed Lib/ stdlib next to the exe.\n" );
		return 2;
	}

	PyGILState_STATE gil = PyGILState_Ensure();

	// Expose the C callback as a module, then create + schedule + run a tasklet around it, so the
	// gtest cases execute with a current tasklet present (just like in the client).
	Py_InitModule( "_trinityintegrationtest", s_methods );
	const int rc = PyRun_SimpleString(
		"import stackless, _trinityintegrationtest\n"
		"stackless.tasklet( _trinityintegrationtest._run_all_tests )()\n"   // create + bind + schedule
		"stackless.run()\n" );                                              // drive it to completion
	if( rc != 0 )
	{
		PyErr_Print();
		fprintf( stderr, "[integration] failed to run the suite inside a tasklet.\n" );
		g_testResult = 3;
	}

	PyGILState_Release( gil );
	return g_testResult;
}
