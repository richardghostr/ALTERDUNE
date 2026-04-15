import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    id: root
    width: 1100
    height: 720
    minimumWidth: 900
    minimumHeight: 620
    visible: true
    title: "ALTERDUNE — RPG"

    readonly property color cBg:       "#0a0c10"
    readonly property color cBg2:      "#111520"
    readonly property color cBg3:      "#181d2a"
    readonly property color cBg4:      "#1e2438"
    readonly property color cBorder:   "#2a3050"
    readonly property color cBorder2:  "#3a4470"
    readonly property color cGold:     "#c9a84c"
    readonly property color cGold2:    "#e8c97a"
    readonly property color cRed:      "#c0392b"
    readonly property color cRedLt:    "#e74c3c"
    readonly property color cTeal:     "#1a7a6e"
    readonly property color cTealLt:   "#25b09e"
    readonly property color cPurple:   "#6c3d9e"
    readonly property color cPurpleLt: "#9b59d0"
    readonly property color cText:     "#d4cfc4"
    readonly property color cText2:    "#8a8580"
    readonly property color cText3:    "#555060"
    readonly property color cGreen:    "#27ae60"
    readonly property color cAmber:    "#e67e22"

    color: cBg

    property int  currentPage:    0
    property bool inCombat:       false
    property bool dataLoaded:     false
    property bool showNameDialog: true

    function hpColor(hp, hpMax) {
        if (hpMax === 0) return cTealLt
        var pct = hp / hpMax
        if (pct > 0.5)  return cTealLt
        if (pct > 0.25) return cAmber
        return cRedLt
    }

    function categoryColor(cat) {
        if (cat === "NORMAL")   return cGreen
        if (cat === "MINIBOSS") return cAmber
        if (cat === "BOSS")     return cRedLt
        return cText2
    }

    function addLog(msg, col) {
        logModel.append({ "txt": msg, "col": col !== undefined ? col : cText2 })
        if (logModel.count > 300) logModel.remove(0)
    }

    Component.onCompleted: {
        if (GameBridge) GameBridge.loadData("../data/items.csv", "../data/monsters.csv")
    }

    Connections {
        target: GameBridge
        function onLogMessage(msg) {
            var col = root.cText2
            if (msg.includes("vaincu") || msg.includes("mort") || msg.includes("Erreur")) col = root.cRedLt
            else if (msg.includes("epargn") || msg.includes("Mercy") || msg.includes("mercy")) col = root.cPurpleLt
            else if (msg.includes("utilise") || msg.includes("soign") || msg.includes("HP")) col = root.cTealLt
            else if (msg.includes("inflige") || msg.includes("degat") || msg.includes("rate")) col = root.cAmber
            else if (msg.includes("Nouvelle") || msg.includes("charg") || msg.includes("combat") || msg.includes("apparait")) col = root.cGold
            root.addLog(msg, col)
        }
        function onStateChanged() { logView.positionViewAtEnd() }
        function onEncounterStarted() {
            root.inCombat = true
            root.currentPage = 1
            root.addLog("─── Nouveau combat ───", root.cGold)
            if (GameBridge) root.addLog(GameBridge.enemyName() + " apparait !", root.cRedLt)
        }
        function onEncounterEnded() {
            root.inCombat = false
            var vic = GameBridge ? GameBridge.victories() : 0
            root.addLog("Victoires : " + vic + " / 10", root.cGold)
            if (vic >= 10) endingOverlay.visible = true
        }
    }

    ListModel { id: logModel }

    // ── DIALOG NOM ────────────────────────────────────────────────
    Rectangle {
        id: nameDialog
        visible: root.showNameDialog
        anchors.fill: parent
        color: root.cBg
        z: 100

        Rectangle {
            anchors.centerIn: parent
            width: 460; height: 340
            color: root.cBg3
            border.color: root.cGold; border.width: 1; radius: 12

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 36; spacing: 20
                Column {
                    Layout.alignment: Qt.AlignHCenter; spacing: 4
                    Text { text: "ALTERDUNE"; font.family: "Georgia"; font.pixelSize: 30; font.letterSpacing: 5; color: root.cGold2; anchors.horizontalCenter: parent.horizontalCenter }
                    Text { text: "Un RPG de la parole et de l'epee"; font.pixelSize: 12; color: root.cText3; font.letterSpacing: 2; anchors.horizontalCenter: parent.horizontalCenter }
                }
                Rectangle { Layout.fillWidth: true; height: 1; color: root.cBorder }
                Text { text: "Entrez votre nom d'aventurier"; color: root.cText2; font.pixelSize: 13; Layout.alignment: Qt.AlignHCenter }
                TextField {
                    id: nameField
                    Layout.fillWidth: true
                    placeholderText: "Votre nom..."
                    maximumLength: 20
                    font.pixelSize: 16; color: root.cText
                    background: Rectangle { color: root.cBg4; border.color: nameField.activeFocus ? root.cGold : root.cBorder; border.width: 1; radius: 6 }
                    Keys.onReturnPressed: {
                        var n = nameField.text.trim()
                        if (n.length === 0) n = "Aventurier"
                        if (GameBridge) GameBridge.newGame(n)
                        root.dataLoaded = true
                        root.showNameDialog = false
                        root.addLog("Bienvenue, " + n + " !", root.cGold2)
                    }
                    Component.onCompleted: forceActiveFocus()
                }
                Rectangle {
                    Layout.fillWidth: true; height: 44; radius: 8
                    color: startHov.containsMouse ? Qt.lighter("#7a5a1a", 1.2) : "#7a5a1a"
                    border.color: root.cGold2; border.width: 1
                    Behavior on color { ColorAnimation { duration: 120 } }
                    Text { anchors.centerIn: parent; text: "Commencer l'aventure"; color: "#0a0800"; font.bold: true; font.pixelSize: 14; font.family: "Georgia" }
                    HoverHandler { id: startHov }
                    TapHandler {
                        id: startBtn
                        onTapped: {
                            var n = nameField.text.trim()
                            if (n.length === 0) n = "Aventurier"
                            if (GameBridge) GameBridge.newGame(n)
                            root.dataLoaded = true
                            root.showNameDialog = false
                            root.addLog("Bienvenue, " + n + " !", root.cGold2)
                        }
                    }
                }
            }
        }
    }

    // ── OVERLAY FIN ──────────────────────────────────────────────
    Rectangle {
        id: endingOverlay
        visible: false; anchors.fill: parent; color: "#cc000000"; z: 90
        Rectangle {
            anchors.centerIn: parent; width: 520; height: 400
            color: root.cBg3; border.color: root.cGold; border.width: 1; radius: 14
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 40; spacing: 14
                Text {
                    Layout.alignment: Qt.AlignHCenter; font.pixelSize: 52
                    text: { var k = GameBridge?GameBridge.playerKills():0; var s = GameBridge?GameBridge.playerSpares():0; return k>0&&s===0?"☠":(s>0&&k===0?"☮":"⚖") }
                    color: { var k = GameBridge?GameBridge.playerKills():0; var s = GameBridge?GameBridge.playerSpares():0; return k>0&&s===0?root.cRedLt:(s>0&&k===0?root.cTealLt:root.cGold2) }
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter; font.family: "Georgia"; font.pixelSize: 22; font.letterSpacing: 3; color: root.cGold2
                    text: { var k = GameBridge?GameBridge.playerKills():0; var s = GameBridge?GameBridge.playerSpares():0; return k>0&&s===0?"FIN GENOCIDAIRE":(s>0&&k===0?"FIN PACIFISTE":"FIN NEUTRE") }
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter; Layout.fillWidth: true
                    wrapMode: Text.WordWrap; horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 13; color: root.cText2; font.italic: true
                    text: { var k = GameBridge?GameBridge.playerKills():0; var s = GameBridge?GameBridge.playerSpares():0;
                        return k>0&&s===0?"Tu as tue chaque creature sans un regard en arriere.\nALTERDUNE tremble a ton passage.":(s>0&&k===0?"Tu as choisi la parole la ou d'autres choisissent l'epee.\nLa paix regne dans ALTERDUNE.":"Tu as tue. Tu as epargne.\nEntre ces deux chemins, tu as trace le tien.") }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter; spacing: 24
                    Repeater {
                        model: [
                            { label: "Tues",      value: GameBridge?GameBridge.playerKills():0 },
                            { label: "Epargnes",  value: GameBridge?GameBridge.playerSpares():0 },
                            { label: "Victoires", value: GameBridge?GameBridge.victories():0 }
                        ]
                        delegate: Rectangle {
                            width: 90; height: 70; color: root.cBg4; border.color: root.cBorder; border.width:1; radius:8
                            Column { anchors.centerIn: parent; spacing:4
                                Text { text: modelData.value; font.pixelSize:22; font.family:"Georgia"; color:root.cGold2; anchors.horizontalCenter:parent.horizontalCenter }
                                Text { text: modelData.label; font.pixelSize:11; color:root.cText3; anchors.horizontalCenter:parent.horizontalCenter }
                            }
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true; height: 42; radius: 8
                    color: replayHov.containsMouse ? Qt.lighter("#7a5a1a",1.2) : "#7a5a1a"
                    border.color: root.cGold2; border.width:1
                    Text { anchors.centerIn:parent; text:"Rejouer"; color:"#0a0800"; font.bold:true; font.pixelSize:14; font.family:"Georgia" }
                    HoverHandler { id: replayHov }
                    TapHandler { onTapped: { endingOverlay.visible=false; logModel.clear(); root.inCombat=false; root.currentPage=0; root.showNameDialog=true } }
                }
            }
        }
    }

    // ── INTERFACE PRINCIPALE ──────────────────────────────────────
    RowLayout {
        anchors.fill: parent; spacing: 0
        visible: !root.showNameDialog

        // SIDEBAR
        Rectangle {
            Layout.preferredWidth: 200; Layout.fillHeight: true
            color: root.cBg2
            Rectangle { width:1; height:parent.height; anchors.right:parent.right; color:root.cBorder }

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 12; spacing: 4

                Column {
                    Layout.fillWidth: true; spacing:2; bottomPadding:12
                    Text { text:"ALTERDUNE"; font.family:"Georgia"; font.pixelSize:16; font.letterSpacing:3; color:root.cGold2; anchors.horizontalCenter:parent.horizontalCenter }
                    Rectangle { width:parent.width; height:1; color:root.cBorder }
                }

                Repeater {
                    model: [
                        { label:"Accueil",      icon:"◈", page:0 },
                        { label:"Combat",       icon:"⚔", page:1 },
                        { label:"Bestiaire",    icon:"◉", page:2 },
                        { label:"Inventaire",   icon:"⬡", page:3 },
                        { label:"Statistiques", icon:"▦", page:4 },
                    ]
                    delegate: Rectangle {
                        Layout.fillWidth: true; height:42; radius:8
                        color: root.currentPage===modelData.page ? root.cBg4 : (navHov.containsMouse ? root.cBg3 : "transparent")
                        border.color: root.currentPage===modelData.page ? root.cBorder2 : "transparent"; border.width:1
                        Behavior on color { ColorAnimation { duration:100 } }
                        RowLayout {
                            anchors.fill:parent; anchors.leftMargin:12; anchors.rightMargin:8; spacing:10
                            Text { text:modelData.icon; font.pixelSize:14; color:root.currentPage===modelData.page?root.cGold2:root.cText2 }
                            Text { text:modelData.label; font.pixelSize:13; color:root.currentPage===modelData.page?root.cGold2:root.cText2 }
                            Rectangle { visible:modelData.page===1&&root.inCombat; width:8;height:8;radius:4;color:root.cRedLt }
                        }
                        HoverHandler { id: navHov }
                        TapHandler { onTapped: {
                            root.currentPage = modelData.page
                            if (modelData.page===1 && !root.inCombat && root.dataLoaded) GameBridge.startEncounter()
                        }}
                    }
                }

                Item { Layout.fillHeight: true }

                Rectangle {
                    Layout.fillWidth: true; height:120
                    color:root.cBg3; border.color:root.cBorder; border.width:1; radius:8
                    ColumnLayout {
                        anchors.fill:parent; anchors.margins:10; spacing:6
                        Text { text:GameBridge?GameBridge.playerName():"—"; font.pixelSize:13; font.family:"Georgia"; color:root.cGold; elide:Text.ElideRight; Layout.fillWidth:true }
                        Column { Layout.fillWidth:true; spacing:3
                            RowLayout {
                                Text { text:"HP"; font.pixelSize:10; color:root.cText3 }
                                Item { Layout.fillWidth:true }
                                Text { text:(GameBridge?GameBridge.playerHP():0)+" / "+(GameBridge?GameBridge.playerHPMax():0); font.pixelSize:10; color:root.cText2 }
                            }
                            Rectangle { width:parent.width; height:5; radius:3; color:root.cBg
                                Rectangle {
                                    width: GameBridge&&GameBridge.playerHPMax()>0?parent.width*(GameBridge.playerHP()/GameBridge.playerHPMax()):0
                                    height:parent.height; radius:3; color:root.hpColor(GameBridge?GameBridge.playerHP():0,GameBridge?GameBridge.playerHPMax():0)
                                    Behavior on width { NumberAnimation { duration:400; easing.type:Easing.OutCubic } }
                                }
                            }
                        }
                        RowLayout { Layout.fillWidth:true
                            Text { text:"Victoires"; font.pixelSize:10; color:root.cText3 }
                            Item { Layout.fillWidth:true }
                            Text { text:(GameBridge?GameBridge.victories():0)+" / 10"; font.pixelSize:11; font.bold:true; color:root.cGold2 }
                        }
                        Rectangle { Layout.fillWidth:true; height:4; radius:3; color:root.cBg
                            Rectangle {
                                width:parent.width*(GameBridge?Math.min(GameBridge.victories(),10)/10:0)
                                height:parent.height; radius:3; color:root.cGold
                                Behavior on width { NumberAnimation { duration:500; easing.type:Easing.OutCubic } }
                            }
                        }
                    }
                }
            }
        }

        // CONTENU
        Rectangle {
            Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg

            StackLayout {
                anchors.fill:parent; currentIndex:root.currentPage

                // PAGE 0 ACCUEIL
                Rectangle { color:"transparent"
                    ColumnLayout { anchors.fill:parent; anchors.margins:24; spacing:14
                        Text { text:"Tableau de bord"; font.family:"Georgia"; font.pixelSize:20; font.letterSpacing:1; color:root.cGold2 }
                        Rectangle { Layout.fillWidth:true; height:1; color:root.cBorder }
                        GridLayout { columns:4; Layout.fillWidth:true; columnSpacing:12; rowSpacing:12
                            Repeater {
                                model: [
                                    { label:"Points de vie",     value:(GameBridge?GameBridge.playerHP():0)+" / "+(GameBridge?GameBridge.playerHPMax():0), accent:root.cTealLt },
                                    { label:"Victoires",          value:(GameBridge?GameBridge.victories():0)+" / 10", accent:root.cGold2 },
                                    { label:"Monstres tues",      value:GameBridge?GameBridge.playerKills():0, accent:root.cRedLt },
                                    { label:"Monstres epargnes",  value:GameBridge?GameBridge.playerSpares():0, accent:root.cPurpleLt },
                                ]
                                delegate: Rectangle { Layout.fillWidth:true; height:80; color:root.cBg3; border.color:root.cBorder; border.width:1; radius:10
                                    Column { anchors.centerIn:parent; spacing:4
                                        Text { text:modelData.value; font.pixelSize:22; font.family:"Georgia"; color:modelData.accent; anchors.horizontalCenter:parent.horizontalCenter }
                                        Text { text:modelData.label; font.pixelSize:11; color:root.cText3; font.letterSpacing:1; anchors.horizontalCenter:parent.horizontalCenter }
                                    }
                                }
                            }
                        }
                        Rectangle { Layout.fillWidth:true; height:60; color:root.cBg3; border.color:root.cBorder; border.width:1; radius:10
                            ColumnLayout { anchors.fill:parent; anchors.margins:14; spacing:8
                                RowLayout { Text{text:"Progression";font.pixelSize:11;color:root.cText3;font.letterSpacing:1} Item{Layout.fillWidth:true} Text{text:(GameBridge?GameBridge.victories():0)+" / 10";font.pixelSize:12;color:root.cGold} }
                                Rectangle { Layout.fillWidth:true; height:8; radius:4; color:root.cBg
                                    Rectangle { width:parent.width*(GameBridge?Math.min(GameBridge.victories(),10)/10:0); height:parent.height; radius:4; color:root.cGold; Behavior on width{NumberAnimation{duration:600;easing.type:Easing.OutCubic}} }
                                }
                            }
                        }
                        Rectangle { Layout.alignment:Qt.AlignHCenter; width:220; height:46; radius:10
                            color:fightHov.containsMouse?"#8a1a1a":"#3a0808"; border.color:root.cRed; border.width:1
                            Behavior on color{ColorAnimation{duration:100}}
                            Text { anchors.centerIn:parent; text:"⚔  Partir au combat"; color:"#ffcccc"; font.pixelSize:15; font.family:"Georgia"; font.bold:true }
                            HoverHandler{id:fightHov}
                            TapHandler{onTapped:{ if(GameBridge) GameBridge.startEncounter() }}
                        }
                        Text { text:"Journal recent"; font.pixelSize:13; color:root.cText3; font.letterSpacing:1 }
                        Rectangle { Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg2; border.color:root.cBorder; border.width:1; radius:8; clip:true
                            ListView { anchors.fill:parent; anchors.margins:10; model:logModel; spacing:2
                                delegate: Text { width:parent?parent.width:0; text:model.txt; color:model.col; font.pixelSize:12; font.italic:true; wrapMode:Text.Wrap }
                                onCountChanged:positionViewAtEnd()
                            }
                        }
                    }
                }

                // PAGE 1 COMBAT
                Rectangle { color:"transparent"
                    ColumnLayout { anchors.fill:parent; spacing:0
                        Rectangle { Layout.fillWidth:true; height:130; color:root.cBg2
                            Rectangle{width:parent.width;height:1;anchors.bottom:parent.bottom;color:root.cBorder}
                            RowLayout { anchors.fill:parent; anchors.margins:20; spacing:20
                                ColumnLayout { Layout.fillWidth:true; spacing:6
                                    Text{text:GameBridge?GameBridge.playerName():"—";font.family:"Georgia";font.pixelSize:15;color:root.cTealLt}
                                    Text{text:(GameBridge?GameBridge.playerHP():0)+" / "+(GameBridge?GameBridge.playerHPMax():0)+" HP";font.pixelSize:12;color:root.cText2}
                                    Rectangle{Layout.fillWidth:true;height:8;radius:4;color:root.cBg
                                        Rectangle{width:GameBridge&&GameBridge.playerHPMax()>0?parent.width*(GameBridge.playerHP()/GameBridge.playerHPMax()):0;height:parent.height;radius:4;color:root.hpColor(GameBridge?GameBridge.playerHP():0,GameBridge?GameBridge.playerHPMax():0);Behavior on width{NumberAnimation{duration:350;easing.type:Easing.OutCubic}}}
                                    }
                                }
                                Text{text:"VS";font.family:"Georgia";font.pixelSize:20;color:root.cBorder2;Layout.alignment:Qt.AlignVCenter}
                                ColumnLayout { Layout.fillWidth:true; spacing:5
                                    Text{text:GameBridge?GameBridge.enemyName():"—";font.family:"Georgia";font.pixelSize:15;color:root.cRedLt}
                                    Text{text:(GameBridge?GameBridge.enemyHP():0)+" / "+(GameBridge?GameBridge.enemyHPMax():0)+" HP";font.pixelSize:12;color:root.cText2}
                                    Rectangle{Layout.fillWidth:true;height:8;radius:4;color:root.cBg
                                        Rectangle{width:GameBridge&&GameBridge.enemyHPMax()>0?parent.width*(GameBridge.enemyHP()/GameBridge.enemyHPMax()):0;height:parent.height;radius:4;color:root.cRedLt;Behavior on width{NumberAnimation{duration:350;easing.type:Easing.OutCubic}}}
                                    }
                                    RowLayout{Text{text:"Mercy";font.pixelSize:10;color:root.cText3}Item{Layout.fillWidth:true}Text{text:(GameBridge?GameBridge.enemyMercy():0)+" / "+(GameBridge?GameBridge.enemyMercyGoal():100);font.pixelSize:10;color:root.cPurpleLt}}
                                    Rectangle{Layout.fillWidth:true;height:5;radius:3;color:root.cBg
                                        Rectangle{width:GameBridge&&GameBridge.enemyMercyGoal()>0?parent.width*(GameBridge.enemyMercy()/GameBridge.enemyMercyGoal()):0;height:parent.height;radius:3;color:root.cPurpleLt;Behavior on width{NumberAnimation{duration:400;easing.type:Easing.OutCubic}}}
                                    }
                                }
                            }
                        }

                        Rectangle { Layout.fillWidth:true; Layout.fillHeight:true; color:root.cBg; clip:true
                            ListView { id:logView; anchors.fill:parent; anchors.margins:16; model:logModel; spacing:3
                                delegate: Text{width:logView.width;text:model.txt;color:model.col;font.pixelSize:13;font.italic:true;wrapMode:Text.Wrap}
                                onCountChanged:positionViewAtEnd()
                            }
                        }

                        Rectangle { Layout.fillWidth:true; color:root.cBg2
                            Rectangle{width:parent.width;height:1;color:root.cBorder}
                            height:combatCol.implicitHeight+24
                            ColumnLayout { id:combatCol; anchors.fill:parent; anchors.margins:16; spacing:10
                                GridLayout { columns:4; Layout.fillWidth:true; columnSpacing:10
                                    Repeater {
                                        model:[
                                            {label:"⚔ FIGHT", bg:"#2a0808",border:root.cRed,    text:"#f09595",action:"fight"},
                                            {label:"◈ ACT",   bg:"#0a1f1e",border:root.cTeal,   text:root.cTealLt,action:"act"},
                                            {label:"⬡ ITEM",  bg:"#1a1400",border:"#633806",    text:"#fac775",action:"item"},
                                            {label:"☮ MERCY", bg:"#1a0d2e",border:root.cPurple, text:root.cPurpleLt,action:"mercy"},
                                        ]
                                        delegate: Rectangle { Layout.fillWidth:true; height:46; radius:8
                                            color:cbHov.containsMouse?Qt.lighter(modelData.bg,1.4):modelData.bg
                                            border.color:modelData.border;border.width:1
                                            opacity:root.inCombat?1:0.4
                                            Behavior on color{ColorAnimation{duration:100}}
                                            Text{anchors.centerIn:parent;text:modelData.label;color:modelData.text;font.family:"Georgia";font.pixelSize:14;font.bold:true;font.letterSpacing:1}
                                            HoverHandler{id:cbHov}
                                            TapHandler{onTapped:{
                                                if(!root.inCombat)return
                                                actPanel.visible=false;itemPanel.visible=false
                                                if(modelData.action==="fight") GameBridge.playerFight()
                                                else if(modelData.action==="act") actPanel.visible=!actPanel.visible
                                                else if(modelData.action==="item") itemPanel.visible=!itemPanel.visible
                                                else if(modelData.action==="mercy") GameBridge.playerMercy()
                                            }}
                                        }
                                    }
                                }

                                Rectangle { id:actPanel; visible:false; Layout.fillWidth:true
                                    height:actInner.implicitHeight+20; color:root.cBg3; border.color:root.cTeal; border.width:1; radius:8
                                    ColumnLayout{id:actInner;anchors.fill:parent;anchors.margins:10;spacing:8
                                        Text{text:"Actions ACT disponibles";font.pixelSize:11;color:root.cTealLt;font.letterSpacing:1}
                                        GridLayout{columns:2;Layout.fillWidth:true;columnSpacing:8;rowSpacing:6
                                            Repeater{model:GameBridge?GameBridge.enemyActs():[]
                                                delegate:Rectangle{Layout.fillWidth:true;height:46;color:aHov.containsMouse?root.cBg4:root.cBg2;border.color:root.cBorder;border.width:1;radius:6;Behavior on color{ColorAnimation{duration:80}}
                                                    RowLayout{anchors.fill:parent;anchors.margins:10;spacing:8
                                                        Column{Layout.fillWidth:true;spacing:2
                                                            Text{text:modelData.id;font.pixelSize:12;font.bold:true;color:root.cTealLt}
                                                            Text{text:modelData.text;font.pixelSize:10;color:root.cText3;elide:Text.ElideRight;width:parent.width}
                                                        }
                                                        Rectangle{width:38;height:20;radius:10;color:modelData.mercy>=0?"#173404":"#2a0808"
                                                            Text{anchors.centerIn:parent;text:(modelData.mercy>=0?"+":"")+modelData.mercy;font.pixelSize:10;font.bold:true;color:modelData.mercy>=0?"#97c459":root.cRedLt}
                                                        }
                                                    }
                                                    HoverHandler{id:aHov}
                                                    TapHandler{onTapped:{GameBridge.playerAct(index);actPanel.visible=false}}
                                                }
                                            }
                                        }
                                    }
                                }

                                Rectangle{id:itemPanel;visible:false;Layout.fillWidth:true
                                    height:itemInner.implicitHeight+20;color:root.cBg3;border.color:"#633806";border.width:1;radius:8
                                    ColumnLayout{id:itemInner;anchors.fill:parent;anchors.margins:10;spacing:8
                                        Text{text:"Inventaire";font.pixelSize:11;color:"#fac775";font.letterSpacing:1}
                                        GridLayout{columns:3;Layout.fillWidth:true;columnSpacing:8;rowSpacing:6
                                            Repeater{model:GameBridge?GameBridge.inventory():[]
                                                delegate:Rectangle{Layout.fillWidth:true;height:44;color:modelData.qty>0?(itHov.containsMouse?root.cBg4:root.cBg2):root.cBg;border.color:modelData.qty>0?root.cBorder:"#222";border.width:1;radius:6;opacity:modelData.qty>0?1:0.4;Behavior on color{ColorAnimation{duration:80}}
                                                    Column{anchors.centerIn:parent;spacing:2
                                                        Text{text:modelData.name;font.pixelSize:12;color:"#fac775";anchors.horizontalCenter:parent.horizontalCenter}
                                                        Text{text:"+"+modelData.value+" HP  x"+modelData.qty;font.pixelSize:10;color:root.cText3;anchors.horizontalCenter:parent.horizontalCenter}
                                                    }
                                                    HoverHandler{id:itHov}
                                                    TapHandler{onTapped:{if(modelData.qty>0){GameBridge.playerUseItem(modelData.name);itemPanel.visible=false}}}
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // PAGE 2 BESTIAIRE
                Rectangle{color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Bestiaire";font.family:"Georgia";font.pixelSize:20;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        Text{visible:!GameBridge||GameBridge.bestiaryEntries().length===0;text:"Aucun monstre vaincu pour l'instant.\nLancez un combat pour remplir le bestiaire.";color:root.cText3;font.italic:true;font.pixelSize:13;Layout.alignment:Qt.AlignHCenter;topPadding:40;horizontalAlignment:Text.AlignHCenter}
                        GridView{Layout.fillWidth:true;Layout.fillHeight:true;cellWidth:220;cellHeight:130;clip:true
                            model:GameBridge?GameBridge.bestiaryEntries():[]
                            delegate:Rectangle{width:210;height:120;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:10
                                ColumnLayout{anchors.fill:parent;anchors.margins:12;spacing:6
                                    RowLayout{
                                        Text{text:modelData.name;font.family:"Georgia";font.pixelSize:14;color:root.cText;Layout.fillWidth:true}
                                        Rectangle{width:cBadge.implicitWidth+12;height:18;radius:9;color:modelData.category==="NORMAL"?"#1e2a10":(modelData.category==="MINIBOSS"?"#2a1e0a":"#2a0a0a")
                                            Text{id:cBadge;anchors.centerIn:parent;text:modelData.category;font.pixelSize:9;font.letterSpacing:1;color:root.categoryColor(modelData.category)}
                                        }
                                    }
                                    Text{text:"HP max : "+modelData.hpMax+"   ATK : "+modelData.atk+"   DEF : "+modelData.def;font.pixelSize:11;color:root.cText2}
                                    Rectangle{width:rTxt.implicitWidth+16;height:22;radius:5;color:modelData.spared?"#0a1f1e":"#2a0a0a"
                                        Text{id:rTxt;anchors.centerIn:parent;text:modelData.spared?"Epargne":"Tue";font.pixelSize:11;color:modelData.spared?root.cTealLt:root.cRedLt}
                                    }
                                }
                            }
                        }
                    }
                }

                // PAGE 3 INVENTAIRE
                Rectangle{color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Inventaire";font.family:"Georgia";font.pixelSize:20;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        Text{text:"Utilisez vos objets hors combat pour vous soigner.";font.pixelSize:12;color:root.cText3;font.italic:true}
                        Repeater{model:GameBridge?GameBridge.inventory():[]
                            delegate:Rectangle{Layout.fillWidth:true;height:64;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:10
                                RowLayout{anchors.fill:parent;anchors.margins:16;spacing:16
                                    Column{spacing:3
                                        Text{text:modelData.name;font.pixelSize:15;color:"#fac775"}
                                        Text{text:"Restaure "+modelData.value+" HP";font.pixelSize:11;color:root.cText3}
                                    }
                                    Item{Layout.fillWidth:true}
                                    Text{text:"x "+modelData.qty;font.pixelSize:14;font.bold:true;color:modelData.qty>0?root.cText:root.cText3}
                                    Rectangle{width:90;height:34;radius:6;color:uHov.containsMouse&&modelData.qty>0?root.cTeal:"#0a1f1e";border.color:modelData.qty>0?root.cTeal:root.cBorder;border.width:1;opacity:modelData.qty>0?1:0.4;Behavior on color{ColorAnimation{duration:100}}
                                        Text{anchors.centerIn:parent;text:"Utiliser";font.pixelSize:12;color:root.cTealLt}
                                        HoverHandler{id:uHov}
                                        TapHandler{onTapped:{if(modelData.qty>0&&GameBridge)GameBridge.playerUseItem(modelData.name)}}
                                    }
                                }
                            }
                        }
                        Item{Layout.fillHeight:true}
                    }
                }

                // PAGE 4 STATS
                Rectangle{color:"transparent"
                    ColumnLayout{anchors.fill:parent;anchors.margins:24;spacing:14
                        Text{text:"Statistiques";font.family:"Georgia";font.pixelSize:20;font.letterSpacing:1;color:root.cGold2}
                        Rectangle{Layout.fillWidth:true;height:1;color:root.cBorder}
                        GridLayout{columns:2;Layout.fillWidth:true;columnSpacing:12;rowSpacing:12
                            Repeater{
                                model:[
                                    {label:"Nom",               value:GameBridge?GameBridge.playerName():"—",    accent:root.cGold2},
                                    {label:"HP actuels",        value:(GameBridge?GameBridge.playerHP():0)+" / "+(GameBridge?GameBridge.playerHPMax():0), accent:root.cTealLt},
                                    {label:"Monstres tues",     value:GameBridge?GameBridge.playerKills():0,     accent:root.cRedLt},
                                    {label:"Monstres epargnes", value:GameBridge?GameBridge.playerSpares():0,    accent:root.cPurpleLt},
                                ]
                                delegate:Rectangle{Layout.fillWidth:true;height:70;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:10
                                    Column{anchors.centerIn:parent;spacing:4
                                        Text{text:modelData.value;font.pixelSize:20;font.family:"Georgia";color:modelData.accent;anchors.horizontalCenter:parent.horizontalCenter}
                                        Text{text:modelData.label;font.pixelSize:11;color:root.cText3;font.letterSpacing:1;anchors.horizontalCenter:parent.horizontalCenter}
                                    }
                                }
                            }
                        }
                        Rectangle{Layout.fillWidth:true;height:60;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:10
                            ColumnLayout{anchors.fill:parent;anchors.margins:14;spacing:8
                                RowLayout{Text{text:"Victoires";font.pixelSize:11;color:root.cText3;font.letterSpacing:1}Item{Layout.fillWidth:true}Text{text:(GameBridge?GameBridge.victories():0)+" / 10";font.pixelSize:12;color:root.cGold}}
                                Rectangle{Layout.fillWidth:true;height:8;radius:4;color:root.cBg
                                    Rectangle{width:parent.width*(GameBridge?Math.min(GameBridge.victories(),10)/10:0);height:parent.height;radius:4;color:root.cGold;Behavior on width{NumberAnimation{duration:600;easing.type:Easing.OutCubic}}}
                                }
                            }
                        }
                        Rectangle{Layout.fillWidth:true;height:80;color:root.cBg3;border.color:root.cBorder;border.width:1;radius:10
                            ColumnLayout{anchors.fill:parent;anchors.margins:14;spacing:6
                                Text{text:"FIN PROBABLE";font.pixelSize:10;color:root.cGold;font.letterSpacing:2}
                                Text{font.pixelSize:13;color:root.cText2;font.italic:true
                                    text:{var k=GameBridge?GameBridge.playerKills():0;var s=GameBridge?GameBridge.playerSpares():0;var v=k+s;if(v===0)return "En attente de resultats...";if(k>0&&s===0)return "Vous vous dirigez vers la Fin Genocidaire...";if(s>0&&k===0)return "Vous vous dirigez vers la Fin Pacifiste...";return "Vous vous dirigez vers la Fin Neutre..."}
                                }
                            }
                        }
                        Item{Layout.fillHeight:true}
                    }
                }
            }
        }
    }
}
