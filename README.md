# html.h

A lightweight single-header HTTP library for C that was made for educational purposes.
Supports both **HTTP client** and **HTTP server** functionality with **zero external dependencies**.

## Features

### HTTP Client
- Build and send HTTP requests (`GET`, `POST`, etc.)
- Parse HTTP responses
- Manage headers and body
- Simple API for minimal overhead

### HTTP Server
- Create lightweight HTTP servers with minimal setup
- Register request handlers for specific routes
- Serve static files with automatic `Content-Type` detection
- Built-in error handling for invalid requests

### Utilities
- String builder for efficient text operations
- File reading helpers
- Complete set of HTTP status codes and `Content-Type` definitions

## Usage

Include only declarations:
```c
#include "http.h"
````

Include implementation as well (in **one .c file**):

```c
#define HTTP_IMPLEMENTATION
#include "http.h"
```

## License

MIT License

```
