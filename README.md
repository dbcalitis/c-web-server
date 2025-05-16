# c-web-server
Preview of the website (temporary): https://dbcalitis.github.io/

# TODO – Portfolio Website on Custom C Web Server

## core server
- [X] basic socket setup
  - [X] create socket
  - [X] bind to port (e.g., 80 or 8080)
  - [X] listen and accept connections
- [ ] request handling
  - [X] parse HTTP/1.1 requests (GET only to start)
  - [ ] handle routing for known paths (e.g., `/`, `/projects`, `/about`)
  - [ ] serve static HTML/CSS/JS files
- [ ] response generation
  - [ ] correct HTTP status codes (200, 404, 500)
  - [X] content-type headers (MIME types)
  - [ ] serve index.html as default
- [ ] concurrency (optional for MVP)
  - [ ] basic thread-per-connection model or select()

## development tools
- [ ] configuration
  - [ ] set server port and root directory via config file or CLI arg
- [ ] logging
  - [ ] log incoming requests (method, path, status)
  - [ ] log server errors
- [ ] hot reload (optional)
  - [ ] reload static files or restart server on file change

## system integration
- [ ] simple security
  - [ ] limit request size
  - [ ] avoid directory traversal (`../`)


