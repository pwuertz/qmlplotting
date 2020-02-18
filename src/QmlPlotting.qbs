import qbs
import qbs.FileInfo

LoadableModule {
    Depends { name: "Qt.quick" }
    name: "qmlplotting"
    targetName: "qmlplottingplugin"
    cpp.cxxLanguageVersion: "c++14"

    Group {
        name: "Plugin"
        files: [
            "plugin/plugin.cpp",
            "qmlplotting/*.cpp",
            "qmlplotting/*.h",
        ]
    }

    Group {
        name: "Ressources"
        files: [
            "ressources/qmldir",
            "ressources/*.qml",
            "ressources/*.js",
        ]
        fileTags: ["ressources"]
    }

    Group {
        fileTagsFilter: ["dynamiclibrary", "ressources"]
        qbs.install: true
        qbs.installDir: "QmlPlotting"
        qbs.installPrefix: ""
    }
}
