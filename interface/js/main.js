var currentColor = "255,255,255";
var currentStep = 1;
var numSteps = 5;
var numLeds = 10;
var ledmatrix = new Array();
var interval = 1000;
var autoplay = false;

$(document).ready(function () {

    document.getElementById('files').addEventListener('change', handleOpenFileSelect, false);

    $('.ledbuttonPaint').each(function () {
        $(this).attr("style", "background-color:rgb(" + $(this).attr('value') + ")");
        console.log($(this).attr("style"));
    });

    $("#numsteps").val(numSteps);
    $("#numleds").val(numLeds);
    $("#interval").val(interval);
    SetCurrentColor(currentColor);



    $('#show_preview').on("click", function () {
        if (autoplay===false)
        {
            
            autoplay=true;
            $('#show_preview').html("STOP");
            playPreview();
        }
        else
        {
            autoplay=false;
            $('#show_preview').html("Anteprima");
            MakeMatrixPreview(numLeds, 0);
        }
    });
    $('.ledbuttonPaint').on("click", function () {
        SetCurrentColor($(this).val());
    });
    $("#numleds").on("change", function () {
        numLeds = $("#numleds").val();
        MakeMatrixPreview(numLeds, 0);
    });
    $("#interval").on("change", function () {
        interval = $("#interval").val();
    });
    $("#numsteps").on("change", function () {
        numSteps = $(this).val();
        MakeMatrixPreview($("#numleds").val(), 0);
    });

    $("#currentstep").on("change", function () {
        currentStep = $(this).val();
        MakeMatrixPreview($("#numleds").val(), $(this).val());
    }
    );
    $("#saveproject").on("click", function () {
        MakeDocument();
    });
    MakeMatrixPreview($("#numleds").val(), 0);

});

playPreview= function()
{
    currentStep++;
    if (currentStep>numSteps)
    {
        currentStep=1;
    }
    if (autoplay)
    {
        MakeMatrixPreview(numLeds,currentStep);
        window.setTimeout(playPreview,interval);
    }
    
};


/**
 * 
 * @returns {undefined}
 */
paintLedMatrix = function ()
{
    for (var i in ledmatrix)
    {
        $("#" + i).attr("style", "background-color:rgb(" + ledmatrix[i] + ")");
    }

};

LoadFromTextArea = function ()
{
    ledmatrix = new Array();
    var str = $("#document").val();
    var parser = str.split("\n");
    var line;
    for (var i in parser)
    {
        line = parser[i].split("=");
        try {
            line[1] = line[1].replace("\r", "");
        } catch (e) {
        }
        if (line[0] == "numsteps")
        {
            numSteps = line[1];
        }
        if (line[0] == "numleds")
        {
            numLeds = line[1];
            console.log("numLeds=" + line[1]);
            continue;
        }
        if (line[0] == "interval")
        {
            interval = line[1];
            console.log("interval=" + line[1]);
            continue;
        }
        ledmatrix[line[0]] = line[1];        
    }

    $("#numsteps").val(numSteps);
    $("#numleds").val(numLeds);
    $("#interval").val(interval);
   // alert(str);

    console.log(numSteps);
    MakeMatrixPreview();

};

/**
 * 
 * @returns {undefined}
 */
MakeDocument = function ()
{
    var str = "";

    str = "interval=" + interval;
    str += "\n";
    str += "numleds=" + numLeds;
    str += "\n";
    str += "numsteps=" + numSteps;
    str += "\n";
    for (var step = 1; step <= numSteps; step++)
    {
        for (var i = 1; i <= numLeds; i++)
        {
            var line = "";
            var idmatrix = "step" + step + "_led" + i;
            line = idmatrix + "=" + "0,0,0";
            if (ledmatrix[idmatrix] !== undefined)
            {
                line = idmatrix + "=" + ledmatrix[idmatrix];
            }

            str += line + "\n";
        }

    }


    $("#document").val(str);
    SaveAs();
};

/**
 * 
 * @param {type} color
 * @returns {undefined}
 */
SetCurrentColor = function (color)
{
    currentColor = color;
    $("#currentcolor").attr("style", "background-color:rgb(" + currentColor + ")");
};

/**
 * 
 * @param {type} numleds
 * @returns {undefined}
 */
MakeMatrixPreview = function (numleds, step)
{

    var html = "<hr />";
    var start;
    var end;
    start = step;
    end = step;
    if (step < 1)
    {
        start = 1;
        end = numSteps;
    }
    for (step = start; step <= end; step++)
    {
        html += "<div>Step" + step + "</div>";
        for (var i = 1; i <= numleds; i++)
        {
            var id = "step" + step + "_led" + i;
            var css = "";
            try {
                css = "background-color:rgb(" + ledmatrix[id] + ")";
            } catch (e) {
                css = "";
            }
            html += "<button style=\"" + css + "\" onclick=\"updateLed(this)\" class=\"ledbuttonPreview\" id=\"" + id + "\"></button>";
        }
    }
    $("#ledtable").html(html);

};



/**
 * 
 * @param {type} el
 * @returns {undefined}
 */
updateLed = function (el)
{
    ledmatrix[el.id] = currentColor;
    console.log(ledmatrix);
    if ($(el).attr("style") != "background-color:rgb(" + currentColor + ")")
    {
        $(el).attr("style", "background-color:rgb(" + currentColor + ")");
    } else
    {
        ledmatrix[el.id] = "0,0,0";
        $(el).attr("style", "background-color:rgb(" + "0,0,0" + ")");
    }
};





//--------------open----------------------------------------------------------->

/**
 * 
 * @param {type} evt
 * @returns {unresolved}
 */
function handleOpenFileSelect(evt) {
    var files = evt.target.files; // FileList object
    if (!files.length) {
        return;
    }
    $("#filename").val(escape(files[0].name));
    var reader = new FileReader();
    reader.onload = function (event)
    {
        $("#document").val(event.target.result);
        LoadFromTextArea();
    };
    reader.readAsText(files[0], "UTF-8");
}

var Download = {
    click: function (node) {
        var ev = document.createEvent("MouseEvents");
        ev.initMouseEvent("click", true, false, self, 0, 0, 0, 0, 0, false, false, false, false, 0, null);
        return node.dispatchEvent(ev);
    },
    encode: function (data) {
        return 'data:application/octet-stream;base64,' + btoa(data);
    },
    link: function (data, name) {
        var a = document.createElement('a');
        a.download = name || self.location.pathname.slice(self.location.pathname.lastIndexOf('/') + 1);
        a.href = data || self.location.href;
        return a;
    }
};
Download.save = function (data, name) {
    this.click(
            this.link(
                    this.encode(data),
                    name
                    )
            );
};
/**
 * 
 * @returns {undefined}
 */
function SaveAs()
{
    Download.save($("#document").val(), $("#filename").val());
}


//--------------open-----------------------------------------------------------<