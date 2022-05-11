<?php
if (isset($_POST['submit'])) {
    echo $_POST['submit'];
    echo $_POST['firstname'];
    echo $_POST['lastname'];
    echo '<h3>Form POST Method</h3>';
    $lastname = $_POST['lastname'];
    $firstname = $_POST['firstname'];
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit; }
else
    echo "coucou";
?>