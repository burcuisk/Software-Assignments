
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class FileProcesses {
	
	int fileType;  //command=1,data=2,output=3
	Scanner sc = null;
	File f = null;    
	BufferedWriter write;
	FileWriter fw;
	
	public void openFile(String inputPath, int fileType) throws IOException {
		f = new File(inputPath);
		sc = new Scanner(f);
		this.fileType = fileType;
	}
	public void openFile(String outputPath) throws IOException{
		f=new File(outputPath);
		if(f.exists()) f.delete();
		fw=new FileWriter(f,false);
		write = new BufferedWriter(fw);	
	}
	
	public int getLineNumber() throws FileNotFoundException{
		int x = 0;  
		while (sc.hasNext()) {
			sc.nextLine();
			x++;
		}
		sc = new Scanner(f);
		sc.nextLine();
		return x;
	}
	
	public String[] readFileandSplit() throws IOException {
		
		String line;
		String delimiter = null;
		String[] commands = new String[200];
		if (sc.hasNext()) {
			line = sc.nextLine();
			if(this.fileType == 2) delimiter = "\\|";
			else if(this.fileType == 1) delimiter= " ";
			commands = line.split(delimiter);
			return commands;
		}
		return null;
	}
	public String readLine(){
		String line = null;
		if (sc.hasNext()) 
			line = sc.nextLine();	
		return line;
	}
	
	public void writeFile(String command , String writing) throws IOException {  // write invalid command Data
		write.write("CommandText: " + "\""+ command +"\""+ System.getProperty("line.separator")+System.getProperty("line.separator") );
		write.write( writing +System.getProperty("line.separator")+"---------------------------"+System.getProperty("line.separator"));
		write.flush();
	}
	public void writeFile (long time) throws IOException{  // writes processtime
		write.write("ProcessTime: " + time +" "+ "milliseconds" + System.getProperty("line.separator") +System.getProperty("line.separator"));
		write.flush();
	}
	public void writeFile(String command,Data[] data,String [] writingTypes,int index) throws IOException{ // writes rowsets Data 
		
		String writing="";
		write.write("CommandText: " +"\""+ command +"\""+ System.getProperty("line.separator")+System.getProperty("line.separator")+ "Results: " +System.getProperty("line.separator") );
		write.flush();
		
		if(data.length == 0){
			write.write("Empty Rowset"+System.getProperty("line.separator")+"---------------------------"+System.getProperty("line.separator"));
			write.flush();
			return;
		}
		if(data[0] == null){
			write.write("Empty Rowset"+System.getProperty("line.separator")+"---------------------------"+System.getProperty("line.separator"));
			write.flush();
			return;
		}
		int maxCID = findMax(data,0);
		int maxSSN = findMax(data,5);
		int maxFN = findMax(data,1);
		int maxLN = findMax (data,2);
		int maxCity = findMax(data,3);
		int maxAdr = findMax(data,4);
		int max = 0;
		
		for(int a=0 ; a<writingTypes.length ; a++) {
			
			if(writingTypes[a] == null) break;
			
			// max length of column in rows
			if(writingTypes[a].equals("CID")) max = maxCID;
			else if(writingTypes[a].equals("FirstName")) max = maxFN;
			else if(writingTypes[a].equals("LastName")) max = maxLN;
			else if(writingTypes[a].equals("City")) max = maxCity;
			else if(writingTypes[a].equals("AddressLine1")) max = maxAdr;
			else if(writingTypes[a].equals("SocialSecurityNumber")) max = maxSSN;
			
			writing += writingTypes[a];
			
			for(int b = 0 ; b<max-writingTypes[a].length();b++)
				writing +=" ";	
			writing += "	";
		}
		
		writing += System.getProperty("line.separator");
		write.write(writing);
		write.flush();

		for (int i=0;i<data.length;i++){
			if (data[i] == null) break;
			writing = "";
			for (int a=0;a<writingTypes.length;a++){
				if(writingTypes[a] == null) break;
				if(writingTypes[a].equals("CID")) {
					writing += data[i].getCID();
					for(int b = 0 ; b<maxCID-Integer.toString(data[i].getCID()).length();b++)
						writing +=" ";
				}
				else if (writingTypes[a].equals("SocialSecurityNumber")){
					writing += data[i].getSSN();
					for(int b = 0 ; b<maxSSN-Integer.toString(data[i].getSSN()).length();b++)
						writing +=" ";
				}
				else if(writingTypes[a].equals("FirstName")){
					writing += data[i].getFirstName();
					for(int b = 0 ; b<maxFN-data[i].getFirstName().length();b++)
						writing +=" ";
				}
				else if (writingTypes[a].equals("LastName")){
					writing += data[i].getLastName();
					for(int b = 0 ; b<maxLN-data[i].getLastName().length();b++)
						writing +=" ";
				}
				else if (writingTypes[a].equals("City")){
					writing += data[i].getCity();
					for(int b = 0 ; b<maxCity-data[i].getCity().length();b++)
						writing +=" ";
				}
				else if (writingTypes[a].equals("AddressLine1")){
					writing += data[i].getAdress();
					for(int b = 0 ; b<maxAdr-data[i].getAdress().length();b++)
						writing +=" ";					
				}
				writing+="	";
			}
			writing+=System.getProperty("line.separator");
			write.write(writing);
			write.flush();
		}
		write.write("---------------------------"+System.getProperty("line.separator"));
		write.flush();

	}
	
	public int findMax(Data[] a,int x){ // this function find max length's in rowsets Data
		
		int max = 0;
		int i = 0;
		
		if( x == 0 ) {
			max = 3;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(Integer.toString(a[i].getCID()).length() > max)
					max = Integer.toString(a[i].getCID()).length();
			}
		}
		else if( x == 5 ) { //  find length which is max length SocialSecurityNumber rows Data
			max = 20;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(Integer.toString(a[i].getSSN()).length() > max)
					max = Integer.toString(a[i].getSSN()).length();
			}
		}
		else if( x == 1 ) { // find length which is max length FirstName rows Data
			max = 9;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(a[i].getFirstName().length() > max)
					max = a[i].getFirstName().length();
			}
		}
		else if( x == 2 ) { // find length which is max length LastName rows Data
			max = 8;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(a[i].getLastName().length() > max)
					max = a[i].getLastName().length();
			}
		}
		else if( x == 3 ) { // find length which is max length City rows Data
			max =4;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(a[i].getCity().length() > max)
					max = a[i].getCity().length();
			}
		}
		else if( x == 4 ) { // find length which is max length AddressLine1 rows Data
			max = 12;
			for(i =0 ;i<a.length ; i++) {
				if (a[i] == null) break;
				if(a[i].getAdress().length() > max)
					max = a[i].getAdress().length();
			}
		}
		return max;
		
	}
	
	public void closeFile() {
		sc.close();
	}
	public void closeFile(String output) throws IOException {
		fw.close();
	}
}
