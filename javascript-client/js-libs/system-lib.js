/**
 * @file system-lib.js
 * @brief Utility functions for SEIR Model simulation.
 */

/**
 * @function getRunSimulationURL
 * @brief Inform the link of simulation.
 * @returns {string} URL of simulation.
 */
function getRunSimulationURL(){         return "https://www.dcc.ufrrj.br/marcelo-z/api-run-simulation.php"}

/**
 * @function getKeyName
 * @brief Inform file private key on client browser.
 * @returns {string} private key file name.
 */
function getKeyName(){                  return "privateKeyPem-SEIR-Model"; }


/**
 * @function main_on_load_script
 * @brief This function is responsible for filling the initial parameters of the simulation.Inform file private key on client browser.
 */
function main_on_load_script(){
    // Set default values/initial condition
    const date = Date.now()
    const st_simulation = date + "-SEIR-Model"
    document.getElementById("id_simulationName").value = st_simulation;
    document.getElementById("id_numNodes").value = 10000;
    document.getElementById("id_cellsX").value = 100;
    document.getElementById("id_cellsY").value = 100;
    document.getElementById("id_power").value = 1;
    document.getElementById("id_minConnected").value = 6;
    document.getElementById("id_infected").value = 0.01;
    document.getElementById("id_timesteps").value = 35;

    document.getElementById("id_alphaMi").value = 1.5;
    document.getElementById("id_alphaSig").value = 0.25;
    document.getElementById("id_gammaMi").value = 2;
    document.getElementById("id_gammaSig").value = 0.75;

}


/**
 * @function remove_key
 * @brief Removes key from client browser
 */
function remove_key(){
        const key_name = getKeyName() ;
        //console.log("remove_key", localStorage.getItem(key_name));
        // Optional: check if it was removed
        if (localStorage.getItem(key_name)){
            localStorage.removeItem(key_name);
            alert("Key has been removed!");
        }else{
            alert("there is no key!");
        }
}

/**
 * @function install_key
 * @brief Installs key from client browser
 */
function install_key(){
    const key_name = getKeyName() ;
    if (localStorage.getItem(key_name)){
        alert("Key has been installed!");
        return;
    }

    const popup = window.open("load-key.html", "Popup", "width=640,height=480");
    window.addEventListener("message", (event) => {
        if (event.origin !== window.location.origin) return; // safety check
        //console.log("Received from popup:", event.data);
    });
}

/**
 * @function main_run_simulation
 * @brief This function performs authentication with the server.
 */
async function main_run_simulation(){
    //Get form info
    const key_name = getKeyName();
    const privateKeyPem = localStorage.getItem(key_name);


    if (!privateKeyPem) {
        alert("Please load your private key first.");
        return;
    }

    const privateKey = forge.pki.privateKeyFromPem(privateKeyPem);
    const message = `id::${Date.now()}`; // Unique message (can also be a nonce from the server)
    const md = forge.md.sha256.create();
    md.update(message, "utf8");

    const signature = privateKey.sign(md);

    const signatureBase64 = forge.util.encode64(signature);

    //------------------------------------------------------
        const username = document.getElementById("id_Login").value;

        const dataEnconded = {
            simulationName: document.getElementById("id_simulationName").value,
            numNodes: parseInt(document.getElementById("id_numNodes").value),
            power: parseFloat(document.getElementById("id_power").value),
            minConnected: parseInt(document.getElementById("id_minConnected").value),
            cellsX: parseInt(document.getElementById("id_cellsX").value),
            cellsY: parseInt(document.getElementById("id_cellsY").value),
            infected: parseInt(document.getElementById("id_infected").value),
            timesteps: parseInt(document.getElementById("id_timesteps").value),
            infectedOrder: document.getElementById("infectedOrder").value,
            alphaMi: parseFloat(document.getElementById("id_alphaMi").value),
            alphaSig: parseFloat(document.getElementById("id_alphaSig").value),
            gammaMi: parseFloat(document.getElementById("id_gammaMi").value),
            gammaSig: parseFloat(document.getElementById("id_gammaSig").value),
            verbose: document.getElementById("verbose").checked
        };
         const dataJson = JSON.stringify(dataEnconded, null, 2);

    // Send to server
    const response = await fetch(getRunSimulationURL(), {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            message: message,
            user: username,
            param:dataJson,
            signature: signatureBase64
        })
    });

    //Get server answer
    const result = await response.text();
    const answer = JSON.parse(result);

    if (answer["status"] == "auth_error"){
        const popup = window.open("out-not-found-err.html", "Popup");
        window.addEventListener("message", (event) => {
            if (event.origin !== window.location.origin) return; // safety check
            console.log("Received from popup:", event.data);
        });
        return;
    }
    console.log(answer["status"]);
    console.log(answer["user"]);


    //console.log("Server says:", result);

}


