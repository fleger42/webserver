<?php

echo getenv('REQUEST_METHOD');
echo $_GET['REQUEST_METHOD'];

if(isset($_GET['last_name'])) {
    echo "WORK";
    echo $_GET['last_name'];
}
else
    echo "\nnot present";
?>