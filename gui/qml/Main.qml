import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 1120; height: 740
    minimumWidth: 960; minimumHeight: 660
    visible: true
    title: "ALTERDUNE — RPG Console"
    color: "#0a0c10"

    // ── Palette ──────────────────────────────────────────────────────
    readonly property color cBg:      "#0a0c10"
    readonly property color cBg2:     "#111520"
    readonly property color cBg3:     "#181d2a"
    readonly property color cBg4:     "#1e2438"
    readonly property color cBorder:  "#2a3050"
    readonly property color cBorder2: "#3a4470"
    readonly property color cGold:    "#c9a84c"
    readonly property color cGold2:   "#e8c97a"
    readonly property color cRed:     "#c0392b"
    readonly property color cRedLt:   "#e74c3c"
    readonly property color cTeal:    "#1a7a6e"
    readonly property color cTealLt:  "#25b09e"
    readonly property color cPurple:  "#6c3d9e"
    readonly property color cPurLt:   "#9b59d0"
    readonly property color cText:    "#d4cfc4"
    readonly property color cText2:   "#8a8580"
    readonly property color cText3:   "#555060"
    readonly property color cAmber:   "#e67e22"
    readonly property color cGreen:   "#27ae60"

    // ── UI state ─────────────────────────────────────────────────────
    property int  page: 0   // 0=home 1=combat 2=bestiary 3=inventory 4=stats 5=monsterSelect
    property bool showName: true
    property bool showEnding: false
    property bool showDmgFlash: false
    property bool flashIsPlayer: false
    property int  flashAmount: 0
    property string subMode: ""   // "" | "act" | "item"

    function log(msg, type) {
        var col = root.cText2
        if (type === "player")  col = "#85b7eb"
        if (type === "monster") col = root.cRedLt
        if (type === "act")     col = "#97c459"
        if (type === "mercy")   col = root.cPurLt
        if (type === "heal")    col = root.cTealLt
        if (type === "system")  col = root.cGold
        if (type === "victory") col = "#fac775"
        if (type === "death")   col = root.cRedLt
        if (type === "miss")    col = root.cText3
        if (type === "error")   col = "#f09595"
        logModel.append({ txt: msg, col: col })
        if (logModel.count > 300) logModel.remove(0)
        logView.positionViewAtEnd()
        homeLogView.positionViewAtEnd()
    }

    function hpPct(hp, mx)  { return mx > 0 ? Math.max(0, hp / mx) : 0 }
    function hpCol(hp, mx)  {
        var p = hpPct(hp, mx)
        if (p > 0.5) return root.cTealLt
        if (p > 0.25) return root.cAmber
        return root.cRedLt
    }
    function catCol(cat) {
        if (cat === "NORMAL")   return root.cGreen
        if (cat === "MINIBOSS") return root.cAmber
        return root.cRedLt
    }
    function catBg(cat) {
        if (cat === "NORMAL")   return "#1e2a10"
        if (cat === "MINIBOSS") return "#2a1e0a"
        return "#2a0a0a"
    }

    ListModel { id: logModel }

    function doStart() {
        var n = nameField.text.trim() !== "" ? nameField.text.trim() : "Aventurier"
        GameBridge.newGame(n)
        root.showName = false
        root.log("Bienvenue, " + n + " ! Votre aventure commence.", "system")
    }

    Component.onCompleted: {
        GameBridge.loadData("../data/items.csv", "../data/monsters.csv")
    }

    Connections {
        target: GameBridge
        function onLogMessage(msg, type) { root.log(msg, type) }
        function onStateChanged()        { /* properties auto-update */ }
        function onEncounterStarted()    { root.subMode = ""; root.page = 1 }
        function onEncounterEnded(won)   { if (won) root.subMode = "" }
        function onPlayerDied()          { root.showEnding = true }
        function onGameEnded()           { root.showEnding = true }
        function onDamageFlash(isPlayer, amount) {
            root.flashIsPlayer = isPlayer
            root.flashAmount   = amount
            root.showDmgFlash  = true
            flashTimer.restart()
        }
    }

    Timer { id: flashTimer; interval: 600; onTriggered: root.showDmgFlash = false }

    // ════════════════════════════════════════════════════════════════
    // ÉCRAN SAISIE NOM
    // ════════════════════════════════════════════════════════════════
    Rectangle {
        visible: root.showName
        anchors.fill: parent; color: root.cBg; z: 200

        Rectangle {
            anchors.centerIn: parent; width: 480; height: 360
            color: root.cBg3; border.color: root.cGold; border.width: 1; radius: 14

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 40; spacing: 22

                Column { Layout.alignment: Qt.AlignHCenter; spacing: 6
                    Text { text: "ALTERDUNE"; font.family: "Georgia"; font.pixelSize: 34; font.letterSpacing: 6; color: root.cGold2; anchors.horizontalCenter: parent.horizontalCenter }
                    Text { text: "Un RPG de la parole et de l'epee"; font.pixelSize: 12; color: root.cText3; font.letterSpacing: 2; anchors.horizontalCenter: parent.horizontalCenter }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: root.cBorder }

                Text { text: "Entrez votre nom d'aventurier"; color: root.cText2; font.pixelSize: 13; Layout.alignment: Qt.AlignHCenter }

                Rectangle {
                    Layout.fillWidth: true; height: 46; radius: 8
                    color: root.cBg4; border.color: nameField.activeFocus ? root.cGold : root.cBorder; border.width: 1
                    Behavior on border.color { ColorAnimation { duration: 150 } }
                    TextInput {
                        id: nameField
                        anchors { fill: parent; leftMargin: 16; rightMargin: 16}
                        verticalAlignment: TextInput.AlignVCenter
                        font.pixelSize: 16; color: root.cText
                        maximumLength: 20
                        Keys.onReturnPressed: doStart()
                        Component.onCompleted: forceActiveFocus()
                        Text { anchors.fill: parent; verticalAlignment: Text.AlignVCenter; text: "Votre nom..."; color: root.cText3; font.pixelSize: 16; visible: !parent.text }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 46; radius: 8
                    color: sbHov.containsMouse ? Qt.lighter("#7a5a1a",1.2) : "#7a5a1a"
                    border.color: root.cGold2; border.width: 1
                    Behavior on color { ColorAnimation { duration: 100 } }
                    Text { anchors.centerIn: parent; text: "Commencer l'aventure"; color: "#0a0800"; font.bold: true; font.pixelSize: 15; font.family: "Georgia" }
                    HoverHandler { id: sbHov }
                    TapHandler { onTapped: doStart() }
                }
            }
        }

    }

    // ════════════════════════════════════════════════════════════════
    // OVERLAY FIN DE PARTIE
    // ════════════════════════════════════════════════════════════════
    Rectangle {
        visible: root.showEnding
        anchors.fill: parent; color: "#d0000000"; z: 150

        Rectangle {
            anchors.centerIn: parent; width: 540; height: 420
            color: root.cBg3; border.color: root.cGold; border.width: 1; radius: 16

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 40; spacing: 14

                Text {
                    Layout.alignment: Qt.AlignHCenter; font.pixelSize: 56
                    text: { if (!root.showEnding) return "⚖"; var t = GameBridge.endingType(); return t==="genocide"?"☠":t==="pacifist"?"☮":t==="death"?"†":"⚖" }
                    color: { if (!root.showEnding) return root.cGold2; var t = GameBridge.endingType(); return t==="genocide"||t==="death"?root.cRedLt:t==="pacifist"?root.cTealLt:root.cGold2 }
                }
                Text { Layout.alignment: Qt.AlignHCenter; text: root.showEnding ? GameBridge.endingTitle() : ""; font.family: "Georgia"; font.pixelSize: 24; font.letterSpacing: 3; color: root.cGold2 }
                Text { Layout.alignment: Qt.AlignHCenter; Layout.fillWidth: true; text: root.showEnding ? GameBridge.endingText() : ""; font.pixelSize: 13; color: root.cText2; font.italic: true; wrapMode: Text.WordWrap; horizontalAlignment: Text.AlignHCenter }

                RowLayout { Layout.alignment: Qt.AlignHCenter; spacing: 28
                    Repeater {
                        model: [
                            { l: "Tues",     v: GameBridge.playerKills },
                            { l: "Epargnes", v: GameBridge.playerSpares },
                            { l: "Victoires",v: GameBridge.victories }
                        ]
                        delegate: Rectangle { width: 96; height: 72; color: root.cBg4; border.color: root.cBorder; border.width:1; radius:10
                            Column { anchors.centerIn: parent; spacing:4
                                Text { text: modelData.v; font.pixelSize:24; font.family:"Georgia"; color:root.cGold2; anchors.horizontalCenter:parent.horizontalCenter }
                                Text { text: modelData.l; font.pixelSize:11; color:root.cText3; anchors.horizontalCenter:parent.horizontalCenter }
                            }
                        }
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 44; radius: 8
                    color: rpHov.containsMouse ? Qt.lighter("#7a5a1a",1.2) : "#7a5a1a"
                    border.color: root.cGold2; border.width:1
                    Behavior on color { ColorAnimation { duration:100 } }
                    Text { anchors.centerIn:parent; text:"Rejouer"; color:"#0a0800"; font.bold:true; font.pixelSize:14; font.family:"Georgia" }
                    HoverHandler { id: rpHov }
                    TapHandler { onTapped: {
                        GameBridge.resetGame()
                        logModel.clear()
                        root.showEnding = false
                        root.showName   = true
                        root.page       = 0
                        root.subMode    = ""
                    }}
                }
            }
        }
    }

    // ════════════════════════════════════════════════════════════════
    // FLASH DOMMAGES
    // ════════════════════════════════════════════════════════════════
    Rectangle {
        id: dmgFlashRect
        visible: root.showDmgFlash
        anchors.fill: parent; z: 100
        color: root.flashIsPlayer ? "#40e74c3c" : "#2025b09e"
        opacity: root.showDmgFlash ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    // ════════════════════════════════════════════════════════════════
    // INTERFACE PRINCIPALE
    // ════════════════════════════════════════════════════════════════
    RowLayout {
        anchors.fill: parent; spacing: 0
        visible: !root.showName

        // ── SIDEBAR ──────────────────────────────────────────────────
        Rectangle {
            Layout.preferredWidth: 210; Layout.fillHeight: true
            color: root.cBg2
            Rectangle { width:1; height:parent.height; anchors.right:parent.right; color:root.cBorder }

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 12; spacing: 4

                // Logo
                Column { Layout.fillWidth: true; spacing:4; bottomPadding:14
                    Text { text:"ALTERDUNE"; font.family:"Georgia"; font.pixelSize:17; font.letterSpacing:4; color:root.cGold2; anchors.horizontalCenter:parent.horizontalCenter }
                    Rectangle { width:parent.width; height:1; color:root.cBorder }
                }

                // Nav items
                Repeater {
                    model: [
                        { label:"Accueil",      icon:"◈", page:0 },
                        { label:"Combat",       icon:"⚔", page:1 },
                        { label:"Choisir monstre", icon:"◎", page:5 },
                        { label:"Bestiaire",    icon:"◉", page:2 },
                        { label:"Inventaire",   icon:"⬡", page:3 },
                        { label:"Statistiques", icon:"▦", page:4 },
                    ]
                    delegate: Rectangle {
                        Layout.fillWidth: true; height:44; radius:8
                        color: root.page===modelData.page ? root.cBg4 : (nh.containsMouse ? root.cBg3 : "transparent")
                        border.color: root.page===modelData.page ? root.cBorder2 : "transparent"; border.width:1
                        Behavior on color { ColorAnimation { duration:100 } }

                        RowLayout { anchors.fill:parent; anchors.leftMargin:14; anchors.rightMargin:10; spacing:10
                            Text { text:modelData.icon; font.pixelSize:15; color:root.page===modelData.page?root.cGold2:root.cText2 }
                            Text { text:modelData.label; font.pixelSize:13; color:root.page===modelData.page?root.cGold2:root.cText2; Layout.fillWidth:true }
                            // badge combat actif
                            Rectangle {
                                visible: modelData.page===1 && GameBridge.inCombat
                                width:8;height:8;radius:4; color:root.cRedLt
                                SequentialAnimation on opacity { running:true; loops:Animation.Infinite
                                    NumberAnimation { to:0.2; duration:700 }
                                    NumberAnimation { to:1.0; duration:700 }
                                }
                            }
                        }
                        HoverHandler { id:nh }
                        TapHandler { onTapped: {
                            root.subMode = ""
                            if (modelData.page === 1 && !GameBridge.inCombat) {
                                GameBridge.startEncounter()
                            } else {
                                root.page = modelData.page
                            }
                        }}
                    }
                }

                Item { Layout.fillHeight: true }

                // Mini stats
                Rectangle {
                    Layout.fillWidth: true; height:130
                    color:root.cBg3; border.color:root.cBorder; border.width:1; radius:10

                    ColumnLayout { anchors.fill:parent; anchors.margins:12; spacing:7
                        Text { text:GameBridge.playerName !== "" ? GameBridge.playerName : "—"; font.pixelSize:14; font.family:"Georgia"; color:root.cGold; elide:Text.ElideRight; Layout.fillWidth:true }

                        Column { Layout.fillWidth:true; spacing:3
                            RowLayout {
                                Text { text:"HP"; font.pixelSize:10; color:root.cText3 }
                                Item { Layout.fillWidth:true }
                                Text { text:GameBridge.playerHP+" / "+GameBridge.playerHPMax; font.pixelSize:10; color:root.cText2 }
                            }
                            Rectangle { width:parent.width; height:6; radius:3; color:root.cBg
                                Rectangle {
                                    width: parent.width * root.hpPct(GameBridge.playerHP, GameBridge.playerHPMax)
                                    height:parent.height; radius:3; color:root.hpCol(GameBridge.playerHP, GameBridge.playerHPMax)
                                    Behavior on width { NumberAnimation { duration:350; easing.type:Easing.OutCubic } }
                                }
                            }
                        }

                        RowLayout { Layout.fillWidth:true
                            Text { text:"Victoires"; font.pixelSize:10; color:root.cText3 }
                            Item { Layout.fillWidth:true }
                            Text { text:GameBridge.victories+" / 10"; font.pixelSize:11; font.bold:true; color:root.cGold2 }
                        }
                        Rectangle { Layout.fillWidth:true; height:4; radius:3; color:root.cBg
                            Rectangle {
                                width: parent.width * Math.min(GameBridge.victories, 10) / 10
                                height:parent.height; radius:3; color:root.cGold
                                Behavior on width { NumberAnimation { duration:500; easing.type:Easing.OutCubic } }
                            }
                        }

                        RowLayout { Layout.fillWidth:true
                            Text { text:"Tues"; font.pixelSize:10; color:root.cText3 }
                            Text { text:GameBridge.playerKills; font.pixelSize:11; color:root.cRedLt }
                            Item { Layout.fillWidth:true }
                            Text { text:"Epargnes"; font.pixelSize:10; color:root.cText3 }
                            Text { text:GameBridge.playerSpares; font.pixelSize:11; color:root.cTealLt }
                        }
                    }
                }
            }
        }

        // ── CONTENU ───────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg

            StackLayout {
                anchors.fill:parent; currentIndex:root.page

                // ────────────────────────────────────────────────────
                // PAGE 0 — ACCUEIL
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout { anchors.fill:parent; anchors.margins:24; spacing:14
                        Text { text:"Tableau de bord"; font.family:"Georgia"; font.pixelSize:21; font.letterSpacing:1; color:root.cGold2 }
                        Rectangle { Layout.fillWidth:true; height:1; color:root.cBorder }

                        GridLayout { columns:4; Layout.fillWidth:true; columnSpacing:12; rowSpacing:12
                            Repeater {
                                model: [
                                    { l:"Points de vie",    v:GameBridge.playerHP+" / "+GameBridge.playerHPMax, a:root.cTealLt },
                                    { l:"Victoires",        v:GameBridge.victories+" / 10",                    a:root.cGold2  },
                                    { l:"Monstres tues",    v:GameBridge.playerKills,                           a:root.cRedLt  },
                                    { l:"Monstres epargnes",v:GameBridge.playerSpares,                          a:root.cPurLt  },
                                ]
                                delegate: Rectangle { Layout.fillWidth:true; height:80; color:root.cBg3; border.color:root.cBorder; border.width:1; radius:10
                                    Column { anchors.centerIn:parent; spacing:5
                                        Text { text:modelData.v; font.pixelSize:22; font.family:"Georgia"; color:modelData.a; anchors.horizontalCenter:parent.horizontalCenter }
                                        Text { text:modelData.l; font.pixelSize:11; color:root.cText3; anchors.horizontalCenter:parent.horizontalCenter }
                                    }
                                }
                            }
                        }

                        // Barre victoires
                        Rectangle { Layout.fillWidth:true; height:56; color:root.cBg3; border.color:root.cBorder; border.width:1; radius:10
                            ColumnLayout { anchors.fill:parent; anchors.margins:14; spacing:8
                                RowLayout {
                                    Text { text:"Progression vers la fin"; font.pixelSize:11; color:root.cText3 }
                                    Item { Layout.fillWidth:true }
                                    Text { text:GameBridge.victories+" / 10"; font.pixelSize:12; color:root.cGold }
                                }
                                Rectangle { Layout.fillWidth:true; height:8; radius:4; color:root.cBg
                                    Rectangle {
                                        width: parent.width * Math.min(GameBridge.victories,10)/10
                                        height:parent.height; radius:4; color:root.cGold
                                        Behavior on width { NumberAnimation { duration:600; easing.type:Easing.OutCubic } }
                                    }
                                }
                            }
                        }

                        // Boutons actions rapides
                        RowLayout { spacing:12
                            Rectangle { width:200; height:46; radius:10
                                color:hfh.containsMouse?"#8a1a1a":"#3a0808"; border.color:root.cRed; border.width:1
                                Behavior on color{ColorAnimation{duration:100}}
                                Text { anchors.centerIn:parent; text:"⚔  Combat aleatoire"; color:"#ffcccc"; font.pixelSize:14; font.family:"Georgia"; font.bold:true }
                                HoverHandler{id:hfh}
                                TapHandler { onTapped: { GameBridge.startEncounter() } }
                            }
                            Rectangle { width:200; height:46; radius:10
                                color:hch.containsMouse?"#0a2a20":"#0a1f1e"; border.color:root.cTeal; border.width:1
                                Behavior on color{ColorAnimation{duration:100}}
                                Text { anchors.centerIn:parent; text:"◎  Choisir un monstre"; color:root.cTealLt; font.pixelSize:14; font.family:"Georgia"; font.bold:true }
                                HoverHandler{id:hch}
                                TapHandler { onTapped: { root.page=5 } }
                            }
                        }

                        // Journal
                        Text { text:"Journal de partie"; font.pixelSize:12; color:root.cText3; font.letterSpacing:1 }
                        Rectangle { Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg2; border.color:root.cBorder; border.width:1; radius:8; clip:true
                            ListView { id:homeLogView; anchors.fill:parent; anchors.margins:12; model:logModel; spacing:2
                                delegate: Text { width:parent?parent.width:0; text:model.txt; color:model.col; font.pixelSize:12; font.italic:true; wrapMode:Text.Wrap }
                                onCountChanged: positionViewAtEnd()
                            }
                        }
                    }
                }

                // ────────────────────────────────────────────────────
                // PAGE 1 — COMBAT
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout { anchors.fill:parent; spacing:0

                        // Header VS
                        Rectangle { Layout.fillWidth:true; height:136; color:root.cBg2
                            Rectangle { width:parent.width; height:1; anchors.bottom:parent.bottom; color:root.cBorder }
                            RowLayout { anchors.fill:parent; anchors.margins:20; spacing:20

                                // Joueur
                                ColumnLayout { Layout.fillWidth:true; spacing:5
                                    RowLayout {
                                        Text { text:GameBridge.playerName !== "" ? GameBridge.playerName : "Joueur"; font.family:"Georgia"; font.pixelSize:16; color:root.cTealLt }
                                        Item { Layout.fillWidth:true }
                                        Text {
                                            visible: root.showDmgFlash && root.flashIsPlayer && root.flashAmount>0
                                            text:"-"+root.flashAmount; color:root.cRedLt; font.pixelSize:14; font.bold:true
                                        }
                                    }
                                    RowLayout {
                                        Text { text:GameBridge.playerHP+" / "+GameBridge.playerHPMax+" HP"; font.pixelSize:12; color:root.cText2 }
                                        Item { Layout.fillWidth:true }
                                        Text { text:Math.round(root.hpPct(GameBridge.playerHP,GameBridge.playerHPMax)*100)+"%"; font.pixelSize:11; color:root.hpCol(GameBridge.playerHP,GameBridge.playerHPMax) }
                                    }
                                    Rectangle { Layout.fillWidth:true; height:10; radius:5; color:root.cBg
                                        Rectangle {
                                            width: parent.width*root.hpPct(GameBridge.playerHP,GameBridge.playerHPMax)
                                            height:parent.height; radius:5; color:root.hpCol(GameBridge.playerHP,GameBridge.playerHPMax)
                                            Behavior on width{NumberAnimation{duration:350;easing.type:Easing.OutCubic}}
                                        }
                                    }
                                    // items rapides
                                    RowLayout { spacing:6
                                        Repeater { model: GameBridge.inventory()
                                            delegate: Rectangle {
                                                visible: modelData.qty>0; height:24
                                                width: itmLbl.implicitWidth+16; radius:4
                                                color:"#1a1400"; border.color:"#633806"; border.width:1
                                                Text { id:itmLbl; anchors.centerIn:parent; text:modelData.name+" x"+modelData.qty; font.pixelSize:10; color:"#fac775" }
                                                HoverHandler{id:itemQuickHov}
                                                TapHandler { onTapped: {
                                                    if(GameBridge.inCombat) GameBridge.playerUseItem(modelData.name)
                                                }}
                                                ToolTip { visible:itemQuickHov.hovered; text:"Cliquer pour utiliser "+modelData.name+" (+"+modelData.value+" HP)"; delay:400 }
                                            }
                                        }
                                    }
                                }

                                Text { text:"VS"; font.family:"Georgia"; font.pixelSize:22; color:root.cBorder2; Layout.alignment:Qt.AlignVCenter }

                                // Ennemi
                                ColumnLayout { Layout.fillWidth:true; spacing:5
                                    RowLayout {
                                        Text { text:GameBridge.enemyName !== "" ? GameBridge.enemyName : "—"; font.family:"Georgia"; font.pixelSize:16; color:root.cRedLt }
                                        Rectangle { visible:GameBridge.enemyName!==""; width:catLbl.implicitWidth+12; height:18; radius:9; color:root.catBg(GameBridge.enemyCategory)
                                            Text{id:catLbl;anchors.centerIn:parent;text:GameBridge.enemyCategory;font.pixelSize:9;font.letterSpacing:1;color:root.catCol(GameBridge.enemyCategory)}
                                        }
                                        Item { Layout.fillWidth:true }
                                        Text {
                                            visible: root.showDmgFlash && !root.flashIsPlayer && root.flashAmount>0
                                            text:"-"+root.flashAmount; color:root.cTealLt; font.pixelSize:14; font.bold:true
                                        }
                                    }
                                    RowLayout {
                                        Text { text:GameBridge.enemyHP+" / "+GameBridge.enemyHPMax+" HP"; font.pixelSize:12; color:root.cText2 }
                                        Item { Layout.fillWidth:true }
                                        Text { text:Math.round(root.hpPct(GameBridge.enemyHP,GameBridge.enemyHPMax)*100)+"%"; font.pixelSize:11; color:root.hpCol(GameBridge.enemyHP,GameBridge.enemyHPMax) }
                                    }
                                    Rectangle { Layout.fillWidth:true; height:10; radius:5; color:root.cBg
                                        Rectangle {
                                            width: parent.width*root.hpPct(GameBridge.enemyHP,GameBridge.enemyHPMax)
                                            height:parent.height; radius:5; color:root.cRedLt
                                            Behavior on width{NumberAnimation{duration:350;easing.type:Easing.OutCubic}}
                                        }
                                    }
                                    // Mercy
                                    RowLayout {
                                        Text{text:"Mercy";font.pixelSize:10;color:root.cText3}
                                        Item{Layout.fillWidth:true}
                                        Text{text:GameBridge.enemyMercy+" / "+GameBridge.enemyMercyGoal;font.pixelSize:10;color:GameBridge.enemyMercy>=GameBridge.enemyMercyGoal?root.cPurLt:root.cText2}
                                        Text{visible:GameBridge.enemyMercy>=GameBridge.enemyMercyGoal;text:"✓ PRET";font.pixelSize:10;color:root.cPurLt;font.bold:true}
                                    }
                                    Rectangle { Layout.fillWidth:true; height:6; radius:3; color:root.cBg
                                        Rectangle {
                                            width: GameBridge.enemyMercyGoal>0 ? parent.width*(GameBridge.enemyMercy/GameBridge.enemyMercyGoal) : 0
                                            height:parent.height; radius:3; color:root.cPurLt
                                            Behavior on width{NumberAnimation{duration:400;easing.type:Easing.OutCubic}}
                                        }
                                    }
                                }
                            }
                        }

                        // Journal combat
                        Rectangle { Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg; clip:true
                            Text { visible:logModel.count===0; anchors.centerIn:parent; text:"Aucun combat en cours.\nDemarrez un combat depuis le menu."; color:root.cText3; font.italic:true; font.pixelSize:13; horizontalAlignment:Text.AlignHCenter }
                            ListView { id:logView; anchors.fill:parent; anchors.margins:16; model:logModel; spacing:3; clip:true
                                delegate: Text { width:logView.width; text:model.txt; color:model.col; font.pixelSize:13; font.italic:true; wrapMode:Text.Wrap }
                                onCountChanged: positionViewAtEnd()
                            }
                        }

                        // Zone actions
                        Rectangle { Layout.fillWidth:true; color:root.cBg2
                            Rectangle{width:parent.width;height:1;color:root.cBorder}
                            height: actionCol.implicitHeight + 24

                            ColumnLayout { id:actionCol; anchors.fill:parent; anchors.margins:16; spacing:10

                                // Message si pas en combat
                                Rectangle { visible: !GameBridge.inCombat; Layout.fillWidth:true; height:46; radius:8
                                    color:startHov.containsMouse?"#8a1a1a":"#3a0808"; border.color:root.cRed; border.width:1
                                    Behavior on color{ColorAnimation{duration:100}}
                                    Text{anchors.centerIn:parent;text:"⚔  Demarrer un combat aleatoire";color:"#ffcccc";font.pixelSize:14;font.family:"Georgia";font.bold:true}
                                    HoverHandler{id:startHov}
                                    TapHandler{onTapped:{GameBridge.startEncounter()}}
                                }

                                // 4 boutons d'action — visibles seulement en combat
                                GridLayout { visible: GameBridge.inCombat; columns:4; Layout.fillWidth:true; columnSpacing:10
                                    Repeater {
                                        model:[
                                            {l:"⚔ FIGHT", bg:"#2a0808",bc:root.cRed,    tc:"#f09595",a:"fight"},
                                            {l:"◈ ACT",   bg:"#0a1f1e",bc:root.cTeal,   tc:root.cTealLt,a:"act"},
                                            {l:"⬡ ITEM",  bg:"#1a1400",bc:"#633806",    tc:"#fac775",a:"item"},
                                            {l:"☮ MERCY", bg:"#1a0d2e",bc:root.cPurple, tc:root.cPurLt,a:"mercy"},
                                        ]
                                        delegate: Rectangle {
                                            Layout.fillWidth:true; height:48; radius:8
                                            color: bh.containsMouse ? Qt.lighter(modelData.bg,1.5) : (root.subMode===modelData.a?Qt.lighter(modelData.bg,1.8):modelData.bg)
                                            border.color: root.subMode===modelData.a ? Qt.lighter(modelData.bc,1.4) : modelData.bc; border.width: root.subMode===modelData.a ? 2 : 1
                                            Behavior on color{ColorAnimation{duration:100}}
                                            // Badge "PRET" sur MERCY
                                            Rectangle {
                                                visible: modelData.a==="mercy" && GameBridge.enemyMercy>=GameBridge.enemyMercyGoal
                                                anchors{top:parent.top;right:parent.right;margins:4}
                                                width:30;height:14;radius:7;color:root.cPurLt
                                                Text{anchors.centerIn:parent;text:"OK";font.pixelSize:8;font.bold:true;color:"#1a0d2e"}
                                            }
                                            Text{anchors.centerIn:parent;text:modelData.l;color:modelData.tc;font.family:"Georgia";font.pixelSize:14;font.bold:true;font.letterSpacing:1}
                                            HoverHandler{id:bh}
                                            TapHandler{onTapped:{
                                                if(modelData.a==="fight")      { root.subMode=""; GameBridge.playerFight() }
                                                else if(modelData.a==="mercy") { root.subMode=""; GameBridge.playerMercy() }
                                                else root.subMode = root.subMode===modelData.a ? "" : modelData.a
                                            }}
                                        }
                                    }
                                }

                                // Sous-panneau ACT
                                Rectangle { visible: GameBridge.inCombat && root.subMode==="act"
                                    Layout.fillWidth:true; color:root.cBg3; border.color:root.cTeal; border.width:1; radius:10
                                    height: actSubCol.implicitHeight+20
                                    ColumnLayout { id:actSubCol; anchors.fill:parent; anchors.margins:14; spacing:8
                                        Text { text:"Choisir une action ACT"; font.pixelSize:11; color:root.cTealLt; font.letterSpacing:1 }
                                        GridLayout { columns:2; Layout.fillWidth:true; columnSpacing:8; rowSpacing:8
                                            Repeater { model: GameBridge.enemyActs()
                                                delegate: Rectangle { Layout.fillWidth:true; height:54; radius:8
                                                    color:ah.containsMouse?root.cBg4:root.cBg2; border.color:root.cBorder; border.width:1
                                                    Behavior on color{ColorAnimation{duration:80}}
                                                    RowLayout{anchors.fill:parent;anchors.margins:12;spacing:10
                                                        Column{Layout.fillWidth:true;spacing:3
                                                            Text{text:modelData.id;font.pixelSize:13;font.bold:true;color:root.cTealLt}
                                                            Text{text:modelData.text;font.pixelSize:11;color:root.cText3;elide:Text.ElideRight;width:parent.width}
                                                        }
                                                        Rectangle{width:42;height:22;radius:11;color:modelData.mercy>=0?"#173404":"#2a0808"
                                                            Text{anchors.centerIn:parent;text:(modelData.mercy>=0?"+":"")+modelData.mercy;font.pixelSize:11;font.bold:true;color:modelData.mercy>=0?"#97c459":root.cRedLt}
                                                        }
                                                    }
                                                    HoverHandler{id:ah}
                                                    TapHandler{onTapped:{GameBridge.playerAct(index);root.subMode=""}}
                                                }
                                            }
                                        }
                                    }
                                }

                                // Sous-panneau ITEM
                                Rectangle { visible: GameBridge.inCombat && root.subMode==="item"
                                    Layout.fillWidth:true; color:root.cBg3; border.color:"#633806"; border.width:1; radius:10
                                    height: itemSubCol.implicitHeight+20
                                    ColumnLayout { id:itemSubCol; anchors.fill:parent; anchors.margins:14; spacing:8
                                        Text{text:"Utiliser un item";font.pixelSize:11;color:"#fac775";font.letterSpacing:1}
                                        GridLayout{columns:3;Layout.fillWidth:true;columnSpacing:8;rowSpacing:8
                                            Repeater{model:GameBridge.inventory()
                                                delegate:Rectangle{Layout.fillWidth:true;height:52;radius:8
                                                    color:modelData.qty>0?(ih.containsMouse?root.cBg4:root.cBg2):root.cBg
                                                    border.color:modelData.qty>0?root.cBorder:"#1e1e1e";border.width:1;opacity:modelData.qty>0?1:0.38
                                                    Behavior on color{ColorAnimation{duration:80}}
                                                    Column{anchors.centerIn:parent;spacing:3
                                                        Text{text:modelData.name;font.pixelSize:13;color:"#fac775";anchors.horizontalCenter:parent.horizontalCenter}
                                                        Text{text:"+"+modelData.value+" HP  ×"+modelData.qty;font.pixelSize:11;color:root.cText3;anchors.horizontalCenter:parent.horizontalCenter}
                                                    }
                                                    HoverHandler{id:ih}
                                                    TapHandler{onTapped:{if(modelData.qty>0){GameBridge.playerUseItem(modelData.name);root.subMode=""}}}
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // ────────────────────────────────────────────────────
                // PAGE 2 — BESTIAIRE
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Bestiaire";font.family:"Georgia";font.pixelSize:21;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        RowLayout {
                            Text {
                                text: GameBridge.bestiaryEntries().length + " monstre(s) vaincu(s)"
                                font.pixelSize: 12; color: root.cText3; font.italic: true
                            }
                        }
                        Text {
                            visible: GameBridge.bestiaryEntries().length === 0
                            text: "Aucun monstre vaincu.\nCommencez un combat !"
                            color: root.cText3; font.italic: true; font.pixelSize: 14
                            Layout.alignment: Qt.AlignHCenter; topPadding: 60
                            horizontalAlignment: Text.AlignHCenter
                        }
                        GridView{id:bestGrid;Layout.fillWidth:true;Layout.fillHeight:true;cellWidth:230;cellHeight:135;clip:true
                            model:GameBridge.bestiaryEntries()
                            delegate:Rectangle{width:220;height:122;color:root.cBg3;border.color:modelData.spared?root.cTeal:root.cBorder;border.width:modelData.spared?1:1;radius:10
                                ColumnLayout{anchors.fill:parent;anchors.margins:13;spacing:5
                                    RowLayout{
                                        Text{text:modelData.name;font.family:"Georgia";font.pixelSize:14;color:root.cText;Layout.fillWidth:true}
                                        Rectangle{width:cLbl.implicitWidth+12;height:18;radius:9;color:root.catBg(modelData.category)
                                            Text{id:cLbl;anchors.centerIn:parent;text:modelData.category;font.pixelSize:9;font.letterSpacing:1;color:root.catCol(modelData.category)}
                                        }
                                    }
                                    RowLayout{spacing:16
                                        Text{text:"HP "+modelData.hpMax;font.pixelSize:11;color:root.cText2}
                                        Text{text:"ATK "+modelData.atk;font.pixelSize:11;color:root.cRedLt}
                                        Text{text:"DEF "+modelData.def;font.pixelSize:11;color:root.cTealLt}
                                    }
                                    Rectangle{width:rLbl.implicitWidth+16;height:22;radius:5;color:modelData.spared?"#0a1f1e":"#2a0a0a"
                                        Text{id:rLbl;anchors.centerIn:parent;text:modelData.spared?"☮ Epargne":"☠ Tue";font.pixelSize:11;color:modelData.spared?root.cTealLt:root.cRedLt}
                                    }
                                }
                            }
                        }
                    }
                }

                // ────────────────────────────────────────────────────
                // PAGE 3 — INVENTAIRE
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Inventaire";font.family:"Georgia";font.pixelSize:21;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        Text{text:"Utilisez vos objets ici (hors combat) ou cliquez dessus pendant le combat.";font.pixelSize:12;color:root.cText3;font.italic:true;wrapMode:Text.Wrap;Layout.fillWidth:true}

                        Repeater{model:GameBridge.inventory()
                            delegate:Rectangle{Layout.fillWidth:true;height:70;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:12
                                RowLayout{anchors.fill:parent;anchors.margins:18;spacing:16
                                    Rectangle{width:44;height:44;radius:8;color:"#1a1400";border.color:"#633806";border.width:1
                                        Text{anchors.centerIn:parent;text:"⬡";font.pixelSize:20;color:"#fac775"}
                                    }
                                    Column{spacing:4;Layout.fillWidth:true
                                        Text{text:modelData.name;font.pixelSize:16;color:"#fac775"}
                                        Text{text:"Restaure "+modelData.value+" HP";font.pixelSize:12;color:root.cText3}
                                    }
                                    Text{text:"× "+modelData.qty;font.pixelSize:16;font.bold:true;color:modelData.qty>0?root.cText:root.cText3}
                                    Rectangle{width:100;height:38;radius:8
                                        color:useH.containsMouse&&modelData.qty>0?root.cTeal:"#0a1f1e"
                                        border.color:modelData.qty>0?root.cTeal:root.cBorder;border.width:1
                                        opacity:modelData.qty>0?1:0.4
                                        Behavior on color{ColorAnimation{duration:100}}
                                        Text{anchors.centerIn:parent;text:"Utiliser";font.pixelSize:13;color:root.cTealLt}
                                        HoverHandler{id:useH}
                                        TapHandler{onTapped:{if(modelData.qty>0)GameBridge.playerUseItem(modelData.name)}}
                                    }
                                }
                            }
                        }
                        Item{Layout.fillHeight:true}
                    }
                }

                // ────────────────────────────────────────────────────
                // PAGE 4 — STATISTIQUES
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Statistiques";font.family:"Georgia";font.pixelSize:21;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}

                        GridLayout{columns:2;Layout.fillWidth:true;columnSpacing:12;rowSpacing:12
                            Repeater{
                                model:[
                                    {l:"Nom",               v:GameBridge.playerName,                                               a:root.cGold2},
                                    {l:"HP actuels",        v:GameBridge.playerHP+" / "+GameBridge.playerHPMax,                   a:root.cTealLt},
                                    {l:"Monstres tues",     v:GameBridge.playerKills,                                              a:root.cRedLt},
                                    {l:"Monstres epargnes", v:GameBridge.playerSpares,                                             a:root.cPurLt},
                                ]
                                delegate:Rectangle{Layout.fillWidth:true;height:72;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:12
                                    Column{anchors.centerIn:parent;spacing:5
                                        Text{text:modelData.v;font.pixelSize:22;font.family:"Georgia";color:modelData.a;anchors.horizontalCenter:parent.horizontalCenter}
                                        Text{text:modelData.l;font.pixelSize:11;color:root.cText3;anchors.horizontalCenter:parent.horizontalCenter}
                                    }
                                }
                            }
                        }

                        Rectangle{Layout.fillWidth:true;height:62;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:12
                            ColumnLayout{anchors.fill:parent;anchors.margins:14;spacing:8
                                RowLayout{
                                    Text{text:"Victoires";font.pixelSize:11;color:root.cText3}
                                    Item{Layout.fillWidth:true}
                                    Text{text:GameBridge.victories+" / 10";font.pixelSize:12;color:root.cGold}
                                }
                                Rectangle{Layout.fillWidth:true;height:8;radius:4;color:root.cBg
                                    Rectangle{width:parent.width*Math.min(GameBridge.victories,10)/10;height:parent.height;radius:4;color:root.cGold;Behavior on width{NumberAnimation{duration:600;easing.type:Easing.OutCubic}}}
                                }
                            }
                        }

                        Rectangle{Layout.fillWidth:true;height:82;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:12
                            ColumnLayout{anchors.fill:parent;anchors.margins:14;spacing:6
                                Text{text:"FIN PROBABLE";font.pixelSize:10;color:root.cGold;font.letterSpacing:2;font.bold:true}
                                Text{font.pixelSize:14;color:root.cText2;font.italic:true
                                    text:{
                                        var k=GameBridge.playerKills,s=GameBridge.playerSpares,v=k+s
                                        if(v===0)return "En attente de resultats..."
                                        if(k>0&&s===0)return "☠  Vous vous dirigez vers la Fin Genocidaire..."
                                        if(s>0&&k===0)return "☮  Vous vous dirigez vers la Fin Pacifiste..."
                                        return "⚖  Vous vous dirigez vers la Fin Neutre..."
                                    }
                                }
                            }
                        }
                        Item{Layout.fillHeight:true}
                    }
                }

                // ────────────────────────────────────────────────────
                // PAGE 5 — CHOISIR UN MONSTRE
                // ────────────────────────────────────────────────────
                Rectangle { color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Choisir un adversaire";font.family:"Georgia";font.pixelSize:21;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        Text{text:"Cliquez sur un monstre pour lancer le combat directement contre lui.";font.pixelSize:12;color:root.cText3;font.italic:true}

                        GridView{Layout.fillWidth:true;Layout.fillHeight:true;cellWidth:280;cellHeight:160;clip:true
                            model: GameBridge.monsterPool()
                            delegate: Rectangle { width:268;height:148
                                color:mh.containsMouse?root.cBg4:root.cBg3
                                border.color:mh.containsMouse?root.catCol(modelData.category):root.cBorder
                                border.width:mh.containsMouse?2:1; radius:12
                                Behavior on color{ColorAnimation{duration:100}}
                                Behavior on border.color{ColorAnimation{duration:100}}

                                ColumnLayout{anchors.fill:parent;anchors.margins:16;spacing:7
                                    RowLayout{
                                        Text{text:modelData.name;font.family:"Georgia";font.pixelSize:16;color:root.cText;Layout.fillWidth:true}
                                        Rectangle{width:mCat.implicitWidth+12;height:20;radius:10;color:root.catBg(modelData.category)
                                            Text{id:mCat;anchors.centerIn:parent;text:modelData.category;font.pixelSize:10;font.letterSpacing:1;color:root.catCol(modelData.category)}
                                        }
                                    }
                                    RowLayout{spacing:20
                                        Column{spacing:2;Text{text:"HP";font.pixelSize:10;color:root.cText3}Text{text:modelData.hp;font.pixelSize:14;color:root.cTealLt;font.bold:true}}
                                        Column{spacing:2;Text{text:"ATK";font.pixelSize:10;color:root.cText3}Text{text:modelData.atk;font.pixelSize:14;color:root.cRedLt;font.bold:true}}
                                        Column{spacing:2;Text{text:"DEF";font.pixelSize:10;color:root.cText3}Text{text:modelData.def;font.pixelSize:14;color:root.cTealLt;font.bold:true}}
                                    }
                                    Text{text:"ACT : "+modelData.acts.join(", ");font.pixelSize:11;color:root.cText3;elide:Text.ElideRight;Layout.fillWidth:true}
                                    Rectangle{Layout.fillWidth:true;height:30;radius:6
                                        color:mh.containsMouse?root.catCol(modelData.category):"transparent"
                                        border.color:root.catCol(modelData.category);border.width:1
                                        Behavior on color{ColorAnimation{duration:100}}
                                        Text{anchors.centerIn:parent;text:"⚔  Combattre";font.pixelSize:12;font.family:"Georgia";color:mh.containsMouse?"#0a0c10":root.catCol(modelData.category)}
                                    }
                                }
                                HoverHandler{id:mh}
                                TapHandler{onTapped:{GameBridge.startEncounterWith(index)}}
                            }
                        }
                    }
                }

            } // StackLayout
        } // content Rectangle
    } // RowLayout principale
}
