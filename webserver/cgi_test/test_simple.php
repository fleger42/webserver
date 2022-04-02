<?php
echo $_ENV['last_name'];
if(isset($_POST['last_name'])) {
    echo "WORK";
    echo $_POST['last_name'];
}
else
    echo "\nnot present";
?>