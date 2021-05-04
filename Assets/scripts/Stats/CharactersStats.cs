using System.Dynamic;
using UnityEngine;

public class CharactersStats : MonoBehaviour
{
    public int maxHealth;
    public int currentHealth { get; protected set; }
    public Stat damage;
    public Stat armour;



    void Awake()
    {
        currentHealth = maxHealth;
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.T))//need to delete this
        {
            takeDamage(10);
        }
    }

    public virtual int HealthUp(int healthUp)
    {
        if (currentHealth < maxHealth) {
            currentHealth += healthUp;
            currentHealth = Mathf.Clamp(currentHealth, 0, maxHealth);
            return 1;//item is used
        }
        return 0;//item isn't used


    }
    
   public virtual void ExpUp(int expAquired)
    {
        //overloaded only for pl
    }
    

    public virtual void takeDamage(int damage)
    {
        damage -= armour.GetValue();
        damage = Mathf.Clamp(damage, 0, int.MaxValue);

        currentHealth -= damage;
        currentHealth = Mathf.Clamp(currentHealth, 0, maxHealth);
        Debug.Log(transform.name + " takes " + damage + " damage");
    }


    public virtual void Die()
    {
        //Die in some way
        //this method should be overriden 
        Debug.Log(transform.name + " died. ");
    }


}
