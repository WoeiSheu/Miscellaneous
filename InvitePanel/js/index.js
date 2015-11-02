window.addEventListener("load",initAll,false);

var xmlHttp = false;                //xmlHttpRequest对象实例
var user = new Array();          //所有class为user的div元素
var isInvited = [0,0,0,0];         //是否已经邀请
var result = new Object();       //读取外部json文件后数据

function initAll() {
    var allUser = document.getElementsByTagName("div");
    var j = 0;
    for(var i = 0; i < allUser.length; i++) {
        if(allUser[i].className == "user") {
            user[j] = allUser[i];
            user[j].index = j;
            user[j].addEventListener("click",toggleInvite,false);   //将user的div块绑定toggleInvite事件
            j++;
        }
    }

    updateUserInfo();
}

function toggleInvite(e) {
    if(e.target.className == "invite") {        //事件冒泡,判断div按钮并处理
        k = this.index;
        if(isInvited[k] == 0) {
            isInvited[k] = 1;
            e.target.style.background = "#2a2a2a";
            e.target.innerHTML = "取消邀请";
        } else {
            isInvited[k] = 0;
            e.target.style.background = "#0051cf";
            e.target.innerHTML = "邀请回答";
        }
    }

    var invitedNumber = 0;
    for (var i = 0; i < isInvited.length; i++) {
        invitedNumber += isInvited[i];
        if(isInvited[i] == 1 && invitedNumber == 1) {
           document.getElementById("hasInvited").children[0].innerHTML = result.recommended[i].name;
           document.getElementById("hasInvited").children[1].innerHTML = "";
        } else if(isInvited[i] == 1 && invitedNumber == 2) {
           document.getElementById("hasInvited").children[1].innerHTML = "、" + result.recommended[i].name;
        }
    }
    if( invitedNumber != 0 ) {
        document.getElementById("hasInvited").style.display = "inline";
        if(invitedNumber > 2) {
            document.getElementById("number").innerHTML = "等" + invitedNumber + "人";
        } else {
            document.getElementById("number").innerHTML = "";
        }
    } else {
        document.getElementById("hasInvited").style.display = "none";
    }
}

function updateUserInfo() {     //读取外部json文件并更新用户信息
    if( window.XMLHttpRequest ) {
        xmlHttp = new XMLHttpRequest();
    } else {
        if( window.ActiveXObject ) {
            try {
                xmlHttp = new ActiveXObject( "Microsoft.XMLHTTP" );
            } catch( e ) {  }
        }
    }

    if( xmlHttp ) {
        xmlHttp.onreadystatechange = setChange;
        xmlHttp.open( "GET", "invite_panel.json", true );
        xmlHttp.send(null);
    } else {
        //alert("Sorry, but I couldn't create an XMLHttpRequest");
    }

}
function setChange() {
    if( xmlHttp.readyState == 4 ) {
        if( xmlHttp.status == 200 || xmlHttp.status == 0) {
            //var result = xmlHttp.responseText;
            eval("result = " + xmlHttp.responseText);
             getUserInfo();
        } else {
            alert( "There was a problem with the request " + xmlHttp.status );
        }
    }
}
function getUserInfo () {       //获取推荐用户的信息并显示
    for(var iterator = 0; iterator < user.length; iterator++){
        user[iterator].children[0].src = result.recommended[iterator].avatarUrl;    //获取头像(avatar)
        user[iterator].children[1].innerHTML = result.recommended[iterator].name;   //获取姓名(name)
        user[iterator].children[2].innerHTML = result.recommended[iterator].bio;    //获取个人简历(bio)
    }
}