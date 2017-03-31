
public class Data  {
	private int cid;
	private String firstName;
	private String lastName;
	private String city;
	private String adressLine;
	private int socialSecurityNumber;

	public Data(int id,String firstName,String lastName,String city,String adress,int ssn){
		this.cid = id;
		this.firstName = firstName;
		this.lastName = lastName;
		this.city = city;
		this.adressLine = adress;
		this.socialSecurityNumber = ssn;
	}
	
	// getter methods
	public int getCID(){
		return this.cid;
	}
	public int getSSN(){
		return this.socialSecurityNumber;
	}
	public String getFirstName() {
		return this.firstName;
	}
	public String getLastName() {
		return this.lastName;
	}
	public String getCity() {
		return this.city;
	}
	public String getAdress() {
		return this.adressLine;
	}
	


}
