<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Information Propagation Simulation</title>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js"></script>
  <script type="text/javascript">
        function getKeyName()                                   {return "privateKeyPem-SEIR-Model"; }

        function on_load_script(){
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

        function remove_key(){
                const key_name = getKeyName() ;
                console.log("remove_key", localStorage.getItem(key_name));
                // Optional: check if it was removed
                if (localStorage.getItem(key_name)){
                    localStorage.removeItem(key_name);
                    alert("Key has been removed!");
                }else{
                    alert("there is no key!");
                }
        }

        function install_key(){
            const popup = window.open("load-key.html", "Popup", "width=640,height=480");
            window.addEventListener("message", (event) => {
                if (event.origin !== window.location.origin) return; // safety check
                console.log("Received from popup:", event.data);
            });
        }
        function start_simulation(){



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

                console.log("Json file:", dataJson);


        }
  </script>
</head>
<body  onload="on_load_script();">
  <div class="p-3 mb-1 bg-dark text-white text-center" >
    <table style="width:100%">
      <tr>
        <th><h2 class="mb-4">Information Propagation Simulation</h2></th>
        <th><button type="button" class="btn btn-primary btn-light btn-block" id="id_remove_btn" onclick="remove_key();">  <img src="./imgs/garbage_red.svg" alt="Check" width="16" height="16" class="me-2"> Remove key </button></th>
      </tr>
      <tr>
        <th><h2 class="mb-4"> &nbsp; &nbsp;</h2></th>
        <th><button type="button" class="btn btn-primary btn-light btn-block" id="id_install_btn" onclick="install_key();">  <img src="./imgs/install_blue.svg" alt="Check" width="16" height="16" class="me-2"> Install key &nbsp;  &nbsp; </button></th>
      </tr>
    </table>

  </div>
  <!-- -->
  <div class="container my-5">
    <!-- <h2 class="mb-4">Information Propagation Simulation</h2> -->
      <!-- Simulation Info -->
      <div class="mb-3">
        <label for="simulationName" class="form-label">Simulation Name</label>
        <input type="text" class="form-control" id="id_simulationName" name="simulationName" required>
      </div>


      <!-- Network Parameters -->
      <div class="row">
        <div class="col-md-4 mb-3">
          <label for="numNodes" class="form-label">Number of Nodes</label>
          <input type="number" inputmode = "numeric" class="form-control" id="id_numNodes" name="numNodes" required>
        </div>
        <div class="col-md-4 mb-3">
          <label for="power" class="form-label">Power</label>
          <input type="number" inputmode = "numeric" class="form-control" step="any" id="id_power" name="power" required>
        </div>
        <div class="col-md-4 mb-3">
          <label for="minConnected" class="form-label">Minimum Nodes Connected</label>
          <input type="number" inputmode = "numeric" class="form-control" id="id_minConnected" name="minConnected" required>
        </div>
      </div>

      <!-- Grid and Infection -->
      <div class="row">
        <div class="col-md-3 mb-3">
          <label for="cellsX" class="form-label">Cells X</label>
          <input type="number" class="form-control" inputmode = "numeric" id="id_cellsX" name="cellsX" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="cellsY" class="form-label">Cells Y</label>
          <input type="number" class="form-control" inputmode = "numeric" id="id_cellsY" name="cellsY" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="infected" class="form-label">Infected</label>
          <input type="number" class="form-control" id="id_infected" name="infected" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="timesteps" class="form-label">Timesteps</label>
          <input type="number" class="form-control" id="id_timesteps" name="timesteps" required>
        </div>
      </div>

      <!-- Infection Order -->
      <div class="mb-3">
        <label for="infectedOrder" class="form-label">Infected order</label>
        <select class="form-select" id="infectedOrder" name="infectedOrder" required>
          <option value="rand">Random</option>
          <option value="asc">Ascending</option>
          <option value="desc">Descending</option>
        </select>
      </div>

      <!-- Alpha/Gamma Parameters -->
      <div class="row">



        <div class="col-md-3 mb-3">
          <label for="alphaMi" class="form-label">Alpha Rumor μ</label>
          <input type="number" class="form-control" step="any" id="id_alphaMi" name="alphaMi" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="alphaSig" class="form-label">Alpha Rumor σ</label>
          <input type="number" class="form-control" step="any" id="id_alphaSig" name="alphaSig" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="gammaMi" class="form-label">Gamma Rumor μ</label>
          <input type="number" class="form-control" step="any" id="id_gammaMi" name="gammaMi" required>
        </div>
        <div class="col-md-3 mb-3">
          <label for="gammaSig" class="form-label">Gamma Rumor σ</label>
          <input type="number" class="form-control" step="any" id="id_gammaSig" name="gammaSig" required>
        </div>
      </div>

      <!-- Verbose Toggle -->
      <div class="form-check form-switch mb-3">
        <input class="form-check-input" type="checkbox" id="verbose" name="verbose">
        <label class="form-check-label" for="verbose">Verbose Output</label>
      </div>

      <button class="btn btn-primary btn-dark btn-block" type="button" id="id_start_btn" onclick="start_simulation();"><img src="./imgs/start_white.svg" alt="Check" width="16" height="16" class="me-2" > Start</button>



  </div>


</body>
</html>
