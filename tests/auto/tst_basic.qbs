import qbs
import qbs.FileInfo

CppApplication {
    builtByDefault: false
    Depends { name: "Qt"; submodules: [ "quick", "testlib", "qmltest" ] }
    Depends { name: "qmlplotting"}
    cpp.cxxLanguageVersion: "c++17"

    property path qmlplottingImportDir: FileInfo.joinPaths(project.buildDirectory, "install-root")
    property pathList qmlImportPaths: [ qmlplottingImportDir ]
    cpp.defines: [
        "QMLPLOTTING_IMPORT_DIR=\"" + FileInfo.fromWindowsSeparators(qmlplottingImportDir) + "\"",
        "QUICK_TEST_SOURCE_DIR=\"" + FileInfo.fromWindowsSeparators(product.sourceDirectory) + "\"",
    ]

    files: [
        "tst_basic.cpp",
        "tst_basic.qml",
    ]
}
