import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Window {
    width: Screen.width
    height: Screen.height
    visible: true
    color: "lightblue"
    title: qsTr("OpencvToQml")

    Rectangle{
        id: imageRect
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 800
        height: 600

        color: "transparent"
        border.color: "black"
        border.width: 3

        Image{
            id: opencvImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            property bool counter: false
            visible: false
            source: "image://live/image"
            asynchronous: false
            cache: false

            function reload()
            {
                counter = !counter
                source = "image://live/image?id=" + counter
            }

        }
    }


    Rectangle{
        id: startButton
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.height/6 + imageRect.y
        color: "white"
        width: 200
        height: 50
        radius: 10
        Text {
            id: startButtonText
            text: qsTr("Start")
            anchors.centerIn: parent
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent

            onClicked: {
                VideoStreamer.openVideoCamera(videoPath.text)
                opencvImage.visible = true
            }
        }
    }

    TextField{
        id:videoPath
        x: startButton.x - startButton.width*0.5
        y: startButton.y + startButton.height * 2
        placeholderText: "Recording path directory"
        cursorVisible: true

        width: startButton.width * 2



    }

    Rectangle{
        id: startRecordingButton
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.height/2 + imageRect.y
        color: "white"
        width: 200
        height: 50
        radius: 10
        Text {
            id: startRecordingButtonText
            text: qsTr("Start Recorder")
            anchors.centerIn: parent
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent

            onClicked: {
                VideoStreamer.startRecording(videoPath.text)
            }
        }
    }

    Rectangle{
        id: saveButton
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.height/1.5 + imageRect.y
        color: "white"
        width: 200
        height: 50
        radius: 10
        Text {
            id: saveButtonText
            text: qsTr("Save")
            anchors.centerIn: parent
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent

            onClicked: {
                VideoStreamer.saveButton()
            }
        }
    }


    Connections{
        target: liveImageProvider

        function onImageChanged()
        {
            opencvImage.reload()
        }

    }
}
