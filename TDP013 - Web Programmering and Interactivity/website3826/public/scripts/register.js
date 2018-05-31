BASEURL = "http://localhost:3826/";

function checkErrors(callback) {
    var error = false;
    var userNameError = false;

    var userName = document.getElementById("userName").value;
    var email = document.getElementById("email").value;
    var password = document.getElementById("password").value;
    
    if (userName.length < 5 || userName.length > 15) {        
        error = true;
        userNameError = true;        
        var msg = "Username must be between 5 and 15 characters";
        addHTMLMessage("errorUsername", msg, "red");
        
    } else { removeHTMLMessage("errorUsername"); }

    if (password.length < 5 || password.length > 15) {
        error = true;
        var msg = "Password must be between 5 and 15 characters";
        addHTMLMessage("errorPassword", msg, "red");
        
    } else { removeHTMLMessage("errorPassword"); }

    takenEmail(email, function(takenEmail) {
        if (takenEmail) {                    
            error = true;
            if (email.length > 0) {
                addHTMLMessage("errorEmail", "email is taken", "red");
            }
        } else { removeHTMLMessage("errorEmail"); }

        takenName(userName, function(takenUsername) {
            if (takenUsername) {
                error = true;
                if (userName.length > 0) {
                    addHTMLMessage("errorUsername", "Name is taken", "red");
                }
            } else if (!userNameError) { removeHTMLMessage("errorUsername"); }

            callback(error);
            return;
        });
    });
}

function register() {
    checkErrors(function(errors) {
        if (errors) {
            return;
        }
        
        var userName = document.getElementById("userName").value;
        var email = document.getElementById("email").value;
        var password = document.getElementById("password").value;

        $.ajax({
            method: "POST",
            url: BASEURL + "api/register",
            contentType: "application/json",
            data: JSON.stringify({
                userName: userName,
                email: email,
                password: password
            }),
            success: function() {            
                window.location = "/registered";
            },
            error: function(response, textStatus, err) {
                var msg = "Error: " + response.status + ": " + textStatus;
                addHTMLMessage("alertRegister", msg, "red");
            }
        });
    });
}

function takenName(userName, callback) {
    var userName = document.getElementById("userName").value;    
    $.ajax({
        method: "POST",
        url: BASEURL + "api/takenname",
        contentType: "application/json",
        data: JSON.stringify({ userName: userName }),
        success: function(result) {                     
            callback(result == "true");
        },
        error: function(response, textStatus, err) {
            var msg = "Error: " + response.status + ": " + textStatus;
            addHTMLMessage("alertRegister", msg, "red");
        }
    });
}

function takenEmail(email, callback) {
    var email = document.getElementById("email").value;
    $.ajax({
        method: "POST",
        url: BASEURL + "api/takenemail",
        contentType: "application/json",
        data: JSON.stringify({ email: email }),
        success: function(result) {            
            callback(result == "true");
        },
        error: function(response, textStatus, err) {
            var msg = "Error: " + response.status + ": " + textStatus;
            addHTMLMessage("alertRegister", msg, "red");
        }
    });
}

//Get all inputs
var inputs = document.getElementsByTagName("input");
for(var i = 0; i < inputs.length; i++) {

    //Check errors when lost focus
    inputs[i].addEventListener("blur", function(e) {
        checkErrors(function(errors) {
            
        });
    });
}

document.getElementById("registerForm").addEventListener("submit", function(e) {
    register();
    e.preventDefault();    
});