<?php

echo getenv('REQUEST_METHOD');
echo $_POST['REQUEST_METHOD'];

if(isset($_POST['last_name'])) {
    echo "WORK";
    echo $_POST['last_name'];
}
else
    echo "\nnot present";
?>