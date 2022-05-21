<?php
if (isset($_POST['submit'])) {
    echo '<h3>Form POST Method</h3>';
    $lastname = $_POST['lastname'];
    $firstname = $_POST['firstname'];
    echo 'Your name is '. $lastname . ' ' . $firstname;
    exit; }
else
    echo "NO SUBMIT";
?>