import java.io.IOException;

public class Main {

	public static void main(String[] args) throws IOException {
	
		FileProcesses file = new FileProcesses();
		FileProcesses outputFile = new FileProcesses();
		file.openFile(args[1], 2);
	
		int n = file.getLineNumber()-1;
		Data[] data = new Data[n];
		
		long startTime,endTime,totalTime;
		
		saveData(data,file);	
		file.closeFile();
		
		CommandProcesses command = new CommandProcesses(data);
		file.openFile(args[0],1);

		outputFile.openFile("output.txt");

		// read and process commands
		String line = file.readLine(); // skip first line
		while (line != null) {  
			startTime = System.currentTimeMillis();
			command.commandProcess(data,file,line,outputFile,startTime);
			endTime = System.currentTimeMillis();
			totalTime = endTime - startTime;
			outputFile.writeFile(totalTime);
			line = file.readLine();
		}
		outputFile.closeFile("outputfile");
		file.closeFile();
	}
	
	public static void saveData(Data [] data,FileProcesses file) throws NumberFormatException, IOException {
		String [] dataLine = new String[10];
		int id,ssn,i=0;
		Data x;
		
		 // adding Data in array
		while ( (dataLine = file.readFileandSplit()) != null ) {
			id= Integer.parseInt(dataLine[0]);
			ssn= Integer.parseInt(dataLine[5]);
			x = new Data(id,dataLine[1],dataLine[2],dataLine[3],dataLine[4],ssn);
			data[i++] = x;
		}
	}

}
