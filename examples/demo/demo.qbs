import qbs
import qbs.FileInfo

CppApplication {
    Depends { name: "Qt"; submodules: [ "core", "quick" ] }
    Depends { name: "qmlplotting"}
    cpp.cxxLanguageVersion: "c++14"

    property path qmlplottingImportDir: FileInfo.joinPaths(project.buildDirectory, "install-root")
    property pathList qmlImportPaths: [ qmlplottingImportDir ]
    cpp.defines: ["QMLPLOTTING_IMPORT_DIR=\"" + FileInfo.fromWindowsSeparators(qmlplottingImportDir) + "\""]

    files: [
        "demo.cpp",
        "demo.qrc",
    ]
}
