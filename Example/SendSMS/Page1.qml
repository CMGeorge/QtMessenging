import QtQuick 2.7

Page1Form {
    button1.onClicked: {
        //console.log("Button Pressed. Entered text: " + textField1.text);
        QtMessenging.sendMessage("0365400401","Mesaj Test Android");
    }
}
