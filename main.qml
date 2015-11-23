import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

Window
{
    id: main
    visible: true
    minimumHeight: 500
    minimumWidth: main.minimumHeight
    maximumHeight: main.minimumHeight
    maximumWidth: main.minimumWidth

    Rectangle
    {
        id: usearea
        anchors.fill: parent
        Image {
            id: bck
            anchors.fill: usearea
            source: "qrc:/Assets/HuffmanBack.png"
        }

        Row
        {
            id: operations
            anchors.top:  usearea.top
            anchors.left: usearea.left
            anchors.topMargin: usearea.height*(3/10)
            anchors.leftMargin: (usearea.width - width)*(1/2)
            width: compress.width + decompress.width + spacing
            spacing: usearea.width*(1/10)

            Rectangle
            {
                id: compress
                visible: true
                implicitHeight: usearea.height*(25/100)
                implicitWidth: usearea.width*(25/100)
                radius: (compress.width*compress.height*(1/2))
                color: "transparent"
                border.color:
                {
                    height: 5
                    width: height
                    color: "#595959"
                }

                Text
                {
                    id: readableComp
                    text: qsTr("Compress")
                    font.pointSize: 16
                    anchors.horizontalCenter: compress.horizontalCenter
                    anchors.verticalCenter: compress.verticalCenter
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        operations.visible = !operations.visible
                        huffComp.visible = !operations.visible
                    }
                }
            }

            Rectangle
            {
                id: decompress
                implicitHeight: main.height*(25/100)
                implicitWidth: main.width*(25/100)
                radius: width*2
                color: "transparent"
                border.color:
                {
                    height: 5
                    width: height
                    color: "#595959"
                }
                Text
                {
                    id: readableDecomp
                    text: qsTr("Decompress")
                    font.pointSize: 16
                    anchors.verticalCenter: decompress.verticalCenter
                    anchors.horizontalCenter: decompress.horizontalCenter
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {

                        operations.visible = !operations.visible
                        huffDecomp.visible = !operations.visible

                    }
                }
            }
        }

        Rectangle
        {
            id: huffComp
            visible: false
            anchors.fill: parent
            property int rCode: -1

            Text
            {
                id: hcTitle
                text: qsTr("Compress");
                anchors.horizontalCenter: huffComp.horizontalCenter;
                anchors.top: huffComp.top
                font.pointSize: 36
            }

                TextInput
                {
                    id: iOComp
                    property bool firstTimeType: true
                    height: compress.height*(1/4)
                    width: huffComp.width*(7/10) - compBrowserButton.width
                    anchors.top: hcTitle.bottom
                    anchors.topMargin: huffComp.height*(1/5)
                    anchors.left: huffComp.left
                    anchors.leftMargin: huffComp.width*(1/10)

                    Layout.alignment: Qt.AlignCenter
                    color: "black"
                    autoScroll: true
                    focus: false
                    text: qsTr("Type a file's path here")
                    font.family: "Arial"
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 18
                    cursorVisible: false
                    clip: true


                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            if(iOComp.firstTimeType)
                            {
                                iOComp.text = "file:///";
                                iOComp.focus = true;
                                iOComp.cursorVisible = iOComp.focus;
                                iOComp.firstTimeType = false;

                            }
                            else
                                iOComp.focus = iOComp.cursorVisible = true;
                        }
                    }
                }

                TextInput
                {
                    id: iOComp2
                    property bool firstTimeType: true
                    height: compress.height*(1/4)
                    width: huffComp.width*(7/10) - compBrowserButton2.width
                    anchors.top: iOComp.bottom
                    anchors.topMargin: huffComp.height*(1/5)
                    anchors.left: huffComp.left
                    anchors.leftMargin: huffComp.width*(1/10)

                    Layout.alignment: Qt.AlignCenter
                    color: "black"
                    autoScroll: true
                    focus: false
                    text: qsTr("Type out name here")
                    font.family: "Arial"
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 18
                    cursorVisible: false
                    clip: true


                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            if(iOComp2.firstTimeType)
                            {
                                iOComp2.text = "file:///";
                                iOComp2.focus = true;
                                iOComp2.cursorVisible = iOComp2.focus;
                                iOComp2.firstTimeType = false;
                            }
                            else
                                iOComp2.focus = iOComp2.cursorVisible = true;
                        }
                    }
                }

                Rectangle
                {
                    id: compBrowserButton
                    implicitHeight: iOComp.height
                    implicitWidth: compress.width*(1/4)
                    radius: compBrowserButton.width*(1/2)
                    anchors.top: iOComp.top
                    anchors.right: huffComp.right
                    anchors.rightMargin: huffComp.width*(1/10)
                    border.color:
                    {
                        height: 1
                        width: 1
                        color: "black"
                    }

                    Text
                    {
                        anchors.baseline: compBrowserButton.verticalCenter
                        anchors.horizontalCenter: compBrowserButton.horizontalCenter
                        anchors.baselineOffset: compBrowserButton.height - implicitHeight
                        text: "..."
                        font.pointSize: 18
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            compInDialog.visible = !compInDialog.visible;
                        }
                    }

                    FileDialog
                    {
                        id: compInDialog
                        title: "Choose a file to compress..."
                        folder: shortcuts.home
                        nameFilters: ["Bitmap Pictures files (*.bmp)", "Text files (*.txt)", "All files (*)"]
                        visible: false

                        onAccepted:
                        {
                            console.log("You provide that original file:\t" + compInDialog.fileUrl);
                            iOComp.text = compInDialog.fileUrl;
                            iOComp.focus = false;
                            iOComp.firstTimeType = false;
                            iOComp.cursorVisible = iOComp.focus;
                        }
                        onRejected:
                        {
                            console.log("Input file choose canceled");
                        }
                    }
                }

                Rectangle
                {
                    id: compBrowserButton2
                    implicitHeight: iOComp2.height
                    implicitWidth: compress.width*(1/4)
                    radius: compBrowserButton2.width*(1/2)
                    anchors.top: iOComp2.top
                    anchors.right: huffComp.right
                    anchors.rightMargin: huffComp.width*(1/10)
                    border.color:
                    {
                        height: 1
                        width: 1
                        color: "black"
                    }

                    Text
                    {
                        anchors.baseline: compBrowserButton2.verticalCenter
                        anchors.horizontalCenter: compBrowserButton2.horizontalCenter
                        anchors.baselineOffset: compBrowserButton2.height - implicitHeight
                        text: "..."
                        font.pointSize: 18
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            compOutDialog.visible = !compOutDialog.visible;
                        }
                    }

                    FileDialog
                    {
                        id: compOutDialog
                        title: "Choose yours huffed file to be created..."
                        folder: compInDialog.folder
                        selectExisting: false
                        nameFilters: ["Huffed Files (*.huff)"]
                        visible: false

                        onAccepted:
                        {
                            console.log("You want that output file was:\t" + compOutDialog.fileUrl)
                            iOComp2.text = compOutDialog.fileUrl;
                            iOComp2.cursorVisible = iOComp2.focus = false;
                            iOComp2.firstTimeType = false;
                        }
                        onRejected:
                        {
                            console.log("Output file choose canceled");
                        }
                    }
                }

            Rectangle
            {
                id: backButton
                property int lftM: ((huffComp.width - (compress.width + compButton.lftM))*(1/2))
                anchors.bottom: huffComp.bottom
                anchors.bottomMargin: huffComp.height*(1/10)
                anchors.left: huffComp.left
                anchors.leftMargin: lftM
                border.color:
                {
                    height: 1
                    wudth: 1
                    color: "black"
                }
                implicitHeight: compress.height*(1/2)
                implicitWidth: compress.width*(1/2)
                radius: backButton.width*(1/2)

                Text
                {
                    anchors.baseline: backButton.top
                    anchors.horizontalCenter:  backButton.horizontalCenter
                    anchors.baselineOffset: backButton.height - implicitHeight
                    text: "Back"
                    font.family: "Arial"
                    font.pointSize: 14
                }


                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        huffComp.visible = !huffComp.visible
                        operations.visible = !huffComp.visible
                    }
                }
            }

            Rectangle
            {
                id: compButton
                property int lftM: backButton.width*(2/5)
                property string iFUrl: iOComp.getText(0,iOComp.length)
                property string oFUrl: iOComp2.getText(0,iOComp2.length)
                anchors.bottom: backButton.bottom
                anchors.left: backButton.right
                anchors.leftMargin: lftM
                border.color:
                {
                    height: 1
                    width: 1
                    color: "#004c99"
                }

                implicitHeight: backButton.height
                implicitWidth: backButton.width
                radius: backButton.radius

                Text
                {
                    anchors.baseline: compButton.top
                    anchors.horizontalCenter:  compButton.horizontalCenter
                    anchors.baselineOffset: compButton.height - implicitHeight
                    text: "Huff it!"
                    font.family: "Arial"
                    font.pointSize: 14
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        if(!compButton.iFUrl.localeCompare("") | iOComp.firstTimeType)
                        {
                            compWarning.visible = !compWarning.visible;
                        }
                        else
                        {
                            if(!compButton.oFUrl.localeCompare("") | iOComp2.firstTimeType)
                            {
                                if(compButton.iFUrl.search('.'))
                                    compButton.oFUrl = compButton.iFUrl.replace(iOComp.getText(compButton.iFUrl.lastIndexOf('.'),(iOComp.length)),".huff");
                                else
                                    compButton.oFUrl = compButton.iFUrl + ".huff";
                            }

                            else
                            {
                                compButton.oFUrl = iOComp2.getText(0,(iOComp2.text.lastIndexOf('.') > 0)? iOComp2.text.lastIndexOf('.'): iOComp2.text.length);
                                compButton.oFUrl = compButton.oFUrl + ".huff";
                            }

                            compCheck.visible = !compCheck.visible;
                        }
                    }

                    onPressAndHold:
                    {
                        compStatus.visible = true;
                    }
                }
            }

            MessageDialog
            {
                id: compWarning
                title: "No File Selected!"
                text: "Please provide a valid file path to compress."
                icon: StandardIcon.Critical
                visible: false
            }

            MessageDialog
            {
                id: compCheck
                title: "Compress Info CheckOut"
                text: "Please check the info below before press ok!"
                detailedText: "Input File path:\t" + compButton.iFUrl + "\nOutput custom name:\t" + compButton.oFUrl + "\n\n"
                icon: StandardIcon.Question
                standardButtons: StandardButton.Show | StandardButton.Cancel | StandardButton.Ok;
                visible: false

                onAccepted:
                {
                    huffComp.rCode = _huff.compress(compButton.iFUrl, compButton.oFUrl);
                    compStatus.visible = true;
                }
            }

            MessageDialog
            {
                id: compStatus
                property string dfltText: "Compress is "
                title: "Status"
                text: ""
                visible: false
                icon: StandardIcon.Information
                standardButtons: StandardButton.Ok

                onVisibleChanged:
                {
                    if(huffComp.rCode >= 0)
                    {
                        if(huffComp.rCode == 0)
                        {
                            compStatus.text = compStatus.dfltText + "completed with sucess!\n";
                        }
                        else
                            compStatus.text = compStatus.dfltText + "failed\nTo more info see console messages\nSorry! Huffman is Dead (x_X)";
                    }
                    else
                        compStatus.text = compStatus.dfltText + "not performed yet!\n";
                }

                onAccepted:
                {
                    if(huffComp.rCode != -1)
                    {
                        Qt.quit();
                    }
                }
            }
        }

        Rectangle
        {
            id: huffDecomp
            visible: false
            anchors.fill: parent
            property int rCode: -1

            Text {
                id: hdTitle
                text: qsTr("Decompress")
                anchors.horizontalCenter: huffDecomp.horizontalCenter;
                anchors.top: huffDecomp.top
                font.pointSize: 36
            }

            TextInput
            {
                id: iODecomp
                property bool firstTimeType: true
                height: decompress.height*(1/4)
                width: huffDecomp.width*(7/10) - decompBrowserButton.width
                anchors.top: hdTitle.bottom
                anchors.topMargin: huffDecomp.height*(1/5)
                anchors.left: huffDecomp.left
                anchors.leftMargin: huffDecomp.width*(1/10)

                Layout.alignment: Qt.AlignCenter
                color: "black"
                autoScroll: true
                focus: false
                text: qsTr("Type a Huffed file path here")
                font.family: "Arial"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                cursorVisible: false
                clip: true


                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        if(iODecomp.firstTimeType)
                        {
                            iODecomp.text = "file:///";
                            iODecomp.focus = true;
                            iODecomp.cursorVisible = iODecomp.focus;
                            iODecomp.firstTimeType = false;

                        }
                        else
                            iODecomp.focus = iODecomp.cursorVisible = true;
                    }
                }
            }

            TextInput
            {
                id: iODecomp2
                property bool firstTimeType: true
                height: decompress.height*(1/4)
                width: huffDecomp.width*(7/10) - decompBrowserButton2.width
                anchors.top: iODecomp.bottom
                anchors.topMargin: huffDecomp.height*(1/5)
                anchors.left: huffDecomp.left
                anchors.leftMargin: huffDecomp.width*(1/10)

                Layout.alignment: Qt.AlignCenter
                color: "black"
                autoScroll: true
                focus: false
                text: qsTr("Type an Output dir path here")
                font.family: "Arial"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                cursorVisible: false
                clip: true


                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        if(iODecomp2.firstTimeType)
                        {
                            iODecomp2.text = "file:///";
                            iODecomp2.focus = true;
                            iODecomp2.cursorVisible = iODecomp2.focus;
                            iODecomp2.firstTimeType = false;
                        }
                        else
                            iODecomp2.focus = iODecomp2.cursorVisible = true;
                    }
                }
            }

            Rectangle
            {
                id: decompBrowserButton
                implicitHeight: iODecomp.height
                implicitWidth: decompress.width*(1/4)
                radius: decompBrowserButton.width*(1/2)
                anchors.top: iODecomp.top
                anchors.right: huffDecomp.right
                anchors.rightMargin: huffDecomp.width*(1/10)
                border.color:
                {
                    height: 1
                    width: 1
                    color: "black"
                }

                Text
                {
                    anchors.baseline: decompBrowserButton.verticalCenter
                    anchors.horizontalCenter: decompBrowserButton.horizontalCenter
                    anchors.baselineOffset: decompBrowserButton.height - implicitHeight
                    text: "..."
                    font.pointSize: 18
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        decompInDialog.visible = !decompInDialog.visible;
                    }
                }

                FileDialog
                {
                    id: decompInDialog
                    title: "Choose a Huffed file to decompress..."
                    folder: shortcuts.home
                    nameFilters: ["Huffed files (*.huff)"]
                    visible: false

                    onAccepted:
                    {
                        console.log("You provide that huffed file:\t" + decompInDialog.fileUrl);
                        iODecomp.text = decompInDialog.fileUrl;
                        iODecomp.focus = iODecomp.cursorVisible = false;
                        iODecomp.firstTimeType = false;
                    }
                    onRejected:
                    {
                        console.log("Input file choose canceled");
                    }
                }
            }

            Rectangle
            {
                id: decompBrowserButton2
                implicitHeight: iODecomp2.height
                implicitWidth: decompress.width*(1/4)
                radius: decompBrowserButton2.width*(1/2)
                anchors.top: iODecomp2.top
                anchors.right: huffDecomp.right
                anchors.rightMargin: huffDecomp.width*(1/10)
                border.color:
                {
                    height: 1
                    width: 1
                    color: "black"
                }

                Text
                {
                    anchors.baseline: decompBrowserButton2.verticalCenter
                    anchors.horizontalCenter: decompBrowserButton2.horizontalCenter
                    anchors.baselineOffset: decompBrowserButton2.height - implicitHeight
                    text: "..."
                    font.pointSize: 18
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        decompOutDialog.visible = !decompOutDialog.visible;
                    }
                }

                FileDialog
                {
                    id: decompOutDialog
                    title: "Choose a output folder..."
                    folder: decompInDialog.folder
                    selectFolder: true
                    visible: false

                    onAccepted:
                    {
                        console.log("You set the output folder to:\t" + compOutDialog.fileUrl.toString())
                        iODecomp2.text = decompOutDialog.fileUrl;
                        iODecomp2.focus = iOComp2.cursorVisible = false;
                        iODecomp2.firstTimeType = false;
                    }
                    onRejected:
                    {
                        console.log("Output folder choose canceled");
                    }
                }
            }

            Rectangle
            {
                id: backButton2
                property int lftM: ((huffDecomp.width - (decompress.width + decompButton.lftM))*(1/2))
                anchors.bottom: huffDecomp.bottom
                anchors.bottomMargin: huffDecomp.height*(1/10)
                anchors.left: huffDecomp.left
                anchors.leftMargin: lftM
                border.color:
                {
                    height: 1
                    width: 1
                    color: "#595959"
                }
                implicitHeight: decompress.height*(1/2)
                implicitWidth: decompress.width*(1/2)
                radius: backButton2.width*(1/2)

                Text
                {
                    anchors.baseline: backButton2.top
                    anchors.horizontalCenter:  backButton2.horizontalCenter
                    anchors.baselineOffset: backButton2.height - implicitHeight
                    text: "Back"
                    font.family: "Arial"
                    font.pointSize: 14
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        huffDecomp.visible = !huffDecomp.visible
                        operations.visible = !huffDecomp.visible
                    }
                }
            }

            Rectangle
            {
                id: decompButton
                property int lftM: backButton2.width*(2/5)
                property string huffedFile: iODecomp.getText(0,iODecomp.length)
                property string outputFolder: iODecomp2.getText(0,iODecomp2.length)
                anchors.bottom: backButton2.bottom
                anchors.left: backButton2.right
                anchors.leftMargin: lftM
                border.color:
                {
                    height: 1
                    width: height
                    color: "#4c0099"
                }

                implicitHeight: backButton2.height
                implicitWidth: backButton2.width
                radius: backButton2.radius

                Text
                {
                    anchors.baseline: decompButton.top
                    anchors.horizontalCenter:  decompButton.horizontalCenter
                    anchors.baselineOffset: decompButton.height - implicitHeight
                    text: "Dehuff"
                    font.family: "Arial"
                    font.pointSize: 14
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        if(!decompButton.huffedFile.localeCompare("") | iODecomp.firstTimeType)
                        {
                            decompWarning.visible = !decompWarning.visible;
                        }
                        else
                        {
                            if(!decompButton.outputFolder.localeCompare("") | iODecomp2.firstTimeType)
                            {
                                decompButton.outputFolder = decompButton.huffedFile.replace(iODecomp.getText((decompButton.huffedFile.lastIndexOf('/')),(iODecomp.length)),"");
                            }
                            else
                            {
                                decompButton.outputFolder = iODecomp2.getText(0,iODecomp2.length);
                            }

                            decompCheck.visible = !decompCheck.visible;
                        }
                    }

                    onPressAndHold:
                    {
                        decompStatus.visible = true;
                    }
                }
            }

            MessageDialog
            {
                id: decompWarning
                title: "No File Selected!"
                text: "Please provide a valid file path to decompress"
                icon: StandardIcon.Critical
                visible: false
            }

            MessageDialog
            {
                id: decompCheck
                title: "Decompress Info Checkout"
                text: "Please check the info below before press ok!"
                detailedText: "Input File path:\t" + decompButton.huffedFile + "\nOutPut folder:\t" + decompButton.outputFolder +"\n\n"
                icon: StandardIcon.Question
                standardButtons: StandardButton.Show | StandardButton.Cancel | StandardButton.Ok;
                visible: false

                onAccepted:
                {
                    huffDecomp.rCode = _huff.decompress(decompButton.huffedFile, decompButton.outputFolder);
                    decompStatus.visible = true;
                }
            }

            MessageDialog
            {
                id: decompStatus
                property string dfltText: "Decompress is "
                title: "Status"
                text: ""
                visible: false
                icon: StandardIcon.Information
                standardButtons: StandardButton.Ok

                onVisibleChanged:
                {
                    if(huffDecomp.rCode >= 0)
                    {
                        if(huffDecomp.rCode == 0)
                        {
                            decompStatus.text = decompStatus.dfltText + "completed with sucess!\n";
                        }
                        else
                            decompStatus.text = decompStatus.dfltText + "failed\nTo more info see console messages\nSorry! Huffman is Dead (x_X)\n\n";
                    }
                    else
                        decompStatus.text = decompStatus.dfltText + "not performed yet!\n";
                }

                onAccepted:
                {
                    if(huffDecomp.rCode != -1)
                    {
                        Qt.quit();
                    }
                }
            }
        }
    }
}
