//https://ottog2486.wordpress.com/2015/03/29/node-js-code-coverage-with-istanbul-and-mocha/

var request = require("request");
var assert = require("assert");

var localHostUrl = "http://localhost:3826/";

describe("Almost Twitter Test:", function() {
    describe("GET /", function() {        
        it("returns status code 200", function(done) {
            request.get(localHostUrl, function(error, response) {
                assert.equal(200, response.statusCode);
                done();
            });
        });
    });

    describe("GET <missing page>", function() {        
        it("Missing page (404)", function(done) {
            var url = localHostUrl + "toocrazytooexisturl";
            request.get(url, function(error, response) {
                assert.equal(404, response.statusCode);
                done();
            });
        });
    });

    describe("POST <url>", function() {        
        it("Bad method (405)", function(done) {
            var url = localHostUrl + "save?m=messagewillnotgetthrough";
            request.post(url, function(error, response) {
                assert.equal(405, response.statusCode);
                done();
            });
        });
    });

    describe("GET /save", function() {        
        it("Post working message (200)", function(done) {
            var url = localHostUrl + "save?m=test%20post";
            request.get(url, function(error, response) {
                assert.equal(200, response.statusCode);
                done();
            });
        });
        
        it("Post message too long (400)", function(done) {
            var url = localHostUrl + "save?m=";
            for(var i = 0; i < 141; i++) {
                url+="f";
            }
            request.get(url, function(error, response) {
                assert.equal(400, response.statusCode);
                done();
            });
        });
        
        it("Post empty message (400)", function(done) {
            var url = localHostUrl + "save?m=";
            request.get(url, function(error, response) {
                assert.equal(400, response.statusCode);
                done();
            });
        });
        
        it("Missing parameter (400)", function(done) {
            var url = localHostUrl + "save";
            request.get(url, function(error, response) {
                assert.equal(400, response.statusCode);
                done();
            });
        });
    });

    describe("GET /flag", function() {        
        it("Working post (200)", function(done) {
            var messageUrl = localHostUrl + "save/?m=message%20for%20flag";
            request.get(messageUrl, function(error, response, body) {
                var id = JSON.parse(body)["_id"];
                var url = localHostUrl + "flag?ID=" + id;
                request.get(url, function(error, response) {
                    assert.equal(200, response.statusCode);
                    done();
                });
            });                        
        });
        
        it("Id of post not found (500)", function(done) {            
            var url = localHostUrl + "flag?ID=38";
            request.get(url, function(error, response) {
                assert.equal(500, response.statusCode);
                done();
            });                     
        });
        
        it("Missing parameter (400)", function(done) {
            var url = localHostUrl + "flag";
            request.get(url, function(error, response) {
                assert.equal(400, response.statusCode);
                done();
            });                     
        });
    });

    describe("GET /getall", function() {        
        it("No crashes (200)", function(done) {            
            var url = localHostUrl + "getall";
            request.get(url, function(error, response) {
                assert.equal(200, response.statusCode);
                done();
            });
        });
    });
});