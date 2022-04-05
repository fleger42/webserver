<?php


if (isset($_GET['submit'])) {
    var_dump($_GET);
    echo $_GET['submit'];
    echo $_GET['firstname'];
    echo $_GET['lastname'];
    $firstname = $_POST['firstname'];
    $lastname = $_POST['lastname'];
    echo '<h3>Form POST Method</h3>';
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit; }
else
    echo "coucou";
?>