<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Check if name and email are provided
    if (isset($_POST["cookie_name"]) && isset($_POST["cookie_value"]))
    {
        setcookie($_POST["cookie_name"], $_POST["cookie_value"], time() + 3600 * 24 * 7);
    }
    else if (isset($_POST["name_remove"]))
    {
        if (isset($_COOKIE[$_POST["name_remove"]]))
        {
            unset($_COOKIE[$_POST["name_remove"]]);
            setcookie($_POST["name_remove"], '', time() - 3600);
        }
        else
        {
            echo "Cookie does not exist!";
        }
    }
    else if (isset($_POST["remove_all"]))
    {
        foreach ($_COOKIE as $key => $value)
        {
            if ($key !== 'name' && $key !== 'email')
            {
                unset($_COOKIE[$key]);
                setcookie($key, '', time() - 3600);
            }
        }
    }
}
?>




<!DOCTYPE html>
<div>
    <h1>Set Cookies</h1>
    <form method="post" action="">
        <input type="text" name="cookie_name" placeholder="Cookie Name" />
        <input type="text" name="cookie_value" placeholder="Cookie Value" />
        <input type="submit" value="Add Cookie" />
    </form>
    <h1>Remove Cookies</h1>
    <form method="post" action="">
        <input type="text" name="name_remove" placeholder="Cookie Name" />
        <input type="submit" value="Remove" />
    </form>
    <br>
    <br>
    <form method="post" action="">
        <input type="hidden" name="remove_all" value="1" />
        <input type="submit" value="Remove All" />
    </form>
    <h3>Added Cookies : </h3>
    <ul>
        <?php foreach ($_COOKIE as $key => $value): ?>
            <?php if ($key !== 'name' && $key !== 'email'): ?>
                <li><?php echo $key . ': ' . $value; ?></li>
            <?php endif; ?>
        <?php endforeach; ?>
    </ul>
</div>