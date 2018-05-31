BASE_URL = "http://localhost:3826/";

function login() {
    var loginName = document.getElementById("loginName").value;
    var password = document.getElementById("password").value;
    
    if (loginName.length == 0) {        
        addHTMLMessage("errLogin", "Login name is empty", "red");
        return;
    }

    if (password.length == 0) {        
        addHTMLMessage("errLogin", "Password is empty", "red");
        return;
    }

    $.ajax({
        method: "POST",
        url: BASE_URL + "api/login",
        contentType: 'application/json',
        data: JSON.stringify({
            loginName: loginName,            
            password: password
        }),
        success: function(userName) {            
            document.setCookie("userName", userName);
            document.setCookie("loggedIn", true);

            removeHTMLMessage("errLogin");
            window.location = "/" + userName;
        },
        error: function(response, textStatus, err) {
            if (response.status == 401) {
                addHTMLMessage("errLogin", "Invalid credentials", "red");
            }
            else {
                var msg = "Error: " + response.status + ": " + textStatus;
                addHTMLMessage("errLogin", msg, "red");
            }
        }
    });
}


document.getElementById("loginForm").addEventListener("submit", function(e) {
    login();
    e.preventDefault();
});