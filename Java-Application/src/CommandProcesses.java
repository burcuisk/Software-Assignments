
import java.io.IOException;
import java.util.Arrays;

public class CommandProcesses {
	String [] commandClauses;
	String [] writingRows;
	QuickSort sort;
	Search search;
	Data [] writingArray;
	
	public CommandProcesses(Data [] data){
		sort = new QuickSort();
		search = new Search();
		this.commandClauses= new String[5];
		this.writingRows = new String[6];
	}
	
	public void commandProcess(Data[] data,FileProcesses file,String line,FileProcesses outputFile,long startTime) throws IOException{

		String writing;
		String [] d,d2,d3,d4;  // condition arrays.
		writingArray = data;
		int type = -1,andNumb=0,clausesNumber=0,j=0,x=0,index = 0;
		String [] commandClauses = new String[5];
		String [] commands = line.split(" ");  
		
		writingRows = commands[1].split(",");
	
		for (int i =3 ;i<commands.length ;i++){ // conditions start 3. index
			if (andNumb > 4) {
				writing = "Error : Invalid Command!";
				outputFile.writeFile(line,writing);
				return; 
			}
			if( commands[i].equals("AND")){
				andNumb++;
				continue; 
			}
			commandClauses[clausesNumber++] = commands[i++]; // eliminate AND's
		}
	
		while(commandClauses[j] != null) {	// read and process conditions

			d  = commandClauses[j].split(">");
			d2 = commandClauses[j].split("<");
			d3 = commandClauses[j].split("~");
			d4 = commandClauses[j].split("=");
				
			boolean IsValid = commandIsValid(d,d2,d3,d4);
				
			if(!IsValid){
				writing = "Error : Invalid Command!";
				outputFile.writeFile(line, writing);
				return;
			}
			
			if(d.length > 1 || d2.length > 1 || d4.length>1) {  // for integer rows
				
				if(d[0].equals("CID") || d2[0].equals("CID") || d4[0].equals("CID") ) type =0;
				else if(d[0].equals("SocialSecurityNumber") || d2[0].equals("SocialSecurityNumber") || d4[0].equals("SocialSecurityNumber")) type = 5;
				
				sort.quickSort(writingArray, 0, writingArray.length-1, type);
				 
				if (d.length > 1){ // copy > values 
					x = Integer.parseInt(d[1]);
					index = search.BinarySearch(writingArray, 0, writingArray.length-1, x, type,">");
					if (index>writingArray.length-1) index = writingArray.length;
					writingArray= Arrays.copyOfRange(writingArray, index, writingArray.length);
				}
				else if (d2.length > 1){ // copy < values 
					x = Integer.parseInt(d2[1]);
					index = search.BinarySearch(writingArray, 0, writingArray.length-1, x, type,"<");
					writingArray = Arrays.copyOfRange(writingArray,0,index+1);	
				}
				else { // copy equal (=) values
					x= Integer.parseInt(d4[1]);
					index = search.BinarySearch(writingArray, 0, writingArray.length-1, x, type,"=");
					if (index == -1)
						writingArray = Arrays.copyOfRange(writingArray,0,index+1);
					writingArray = search.SequantalSearch(writingArray, x, index, type);		
				}
			}
			
			else if (d3.length>1){		// for string rows
				if(d3[0].equals("FirstName")) type = 1;
				else if (d3[0].equals("LastName")) type = 2;
				else if (d3[0].equals("City")) type = 3;
				else if (d3[0].equals("AddressLine1")) type = 4;
				
				if( j >= 1)  // if isnot first condition
					writingArray = search.SequantalSearch(writingArray,d3[1],type);
				
				else {
					sort.quickSort(writingArray, 0, writingArray.length-1, type);
					index = search.BinarySearch(writingArray, 0, writingArray.length-1,d3[1], type);
					writingArray = Arrays.copyOfRange(writingArray,index,writingArray.length);
					writingArray = search.SequantalSearch(writingArray, d3[1], type);
				}
			}
			j++;
		}
		outputFile.writeFile(line, writingArray, writingRows,0);

	}
	
	public boolean commandIsValid(String [] d,String [] d2,String [] d3,String [] d4){
		if(d.length > 1 || d2.length > 1 || d4.length>1) {
			if(!d[0].equals("CID") && !d[0].equals("SocialSecurityNumber") &&
			   !d2[0].equals("CID") && !d2[0].equals("SocialSecurityNumber") &&
				!d4[0].equals("CID") && !d4[0].equals("SocialSecurityNumber"))
				return false;
			else 
				return true;
		}
		else if(d3.length > 1){
			if(d3[0].equals("CID") || d3.equals("SocialSecurityNumber"))
				return false;
			else 
				return true;
		}
		return false;
		
	}
}
