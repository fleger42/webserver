<?php


if (isset($_GET['submit'])) {
    echo $_GET['submit'];
    echo $_GET['firstname'];
    echo $_GET['lastname'];
    echo '<h3>Form GET Method</h3>';
    $lastname = $_GET['lastname'];
    $firstname = $_GET['firstname'];
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit; }
else
    echo "coucou";
?>