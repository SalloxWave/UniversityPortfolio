function addHTMLMessage(elementId, message, color) {
    var element = document.getElementById(elementId);    
    element.style.color = color;    
    element.innerHTML = message;
}

function removeHTMLMessage(elementId) {
    document.getElementById(elementId).innerHTML = "";
}