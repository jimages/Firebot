/*
 * Date:    Jun.16 2016
 * Author:  Zachary Wang(jimages123@gmail.com)
 * Copyright(c) Under MIT License
 * See the full license in root directory.
 */

/* this sql file initialize the sql file to establish the discrete bot system.*/

-- The flowing table is the network config which is load when firebot start.
CREATE TABLE IF NOT EXISTS firebot_config (
    id INT NOT NULL AUTO_INCREMENT KEY,
    name CHAR(100) NOT NULL UNIQUE,
    value CHAR(100) NOT NULL
);
