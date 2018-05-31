function logout() {
    document.clearCookie("userName");
    document.setCookie("loggedIn", false);
    window.location = "/";
}