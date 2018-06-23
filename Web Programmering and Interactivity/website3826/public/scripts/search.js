function search() {
    var query = document.getElementById("txtQuery").value;
    var limit = document.getElementById("numLimit").value;
    var url = "/search?q=" + query + (limit == 0 ? "" : "&l=" + limit);
    window.location = url;
}

document.getElementById("searchForm").addEventListener("submit", function(e) {
    search();
    e.preventDefault();
});