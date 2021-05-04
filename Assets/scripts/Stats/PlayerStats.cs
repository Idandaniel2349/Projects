using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerStats : CharactersStats
{
    public int level;
    private int experience;
    private int experienceRequired;


    public Text HealthText;
    public Text damageText;
    public Text armourText;
    public Text levelText;

    public Image healthFill;
    public Image levelFill;

    // Start is called before the first frame update
    void Start()
    {
        level = 1;
        experience = 0;
        experienceRequired = 10;

        EquipmentMangaer.instance.onEquipmentChanged += OnEquipmentChanged;

        //Update Ui to initial player values
        levelText.text = this.level.ToString();
        levelFill.fillAmount = (float)experience / (float)experienceRequired;
        healthFill.fillAmount = (float) currentHealth / (float)maxHealth;
        HealthText.text = this.currentHealth.ToString();


        damageText.text = "Damage: " + this.damage.GetValue();
        armourText.text = "Armour: " + this.armour.GetValue();

    }
    
    public void LevelUp()
    {
        AudioManager.instance.Play("LevelUp");

        level++;
        experience = experienceRequired - experience;
        experienceRequired += 10;
        maxHealth += 10;
        currentHealth = maxHealth;
        damage.AddToBaseValue(5);
        armour.AddToBaseValue(1);

        levelText.text = this.level.ToString();
        healthFill.fillAmount = (float)currentHealth / (float)maxHealth;
        HealthText.text = this.currentHealth.ToString();

        damageText.text = "Damage: " + this.damage.GetValue();
        armourText.text = "Armour: " + this.armour.GetValue();
    }

    public override void ExpUp(int expAquired)
    {
        experience += expAquired;
        if (experience >= experienceRequired)
        {
            LevelUp();
        }
        levelFill.fillAmount = (float)experience / (float)experienceRequired;

    }
    
    

    void OnEquipmentChanged(Equipment newItem, Equipment oldItem)
    {
        if (newItem != null)
        {
            armour.AddModifier(newItem.armourModifier);
            damage.AddModifier(newItem.damageModifier);
            damageText.text = "Damage: " + this.damage.GetValue();
            armourText.text = "Armour: " + this.armour.GetValue();
        }
        if (oldItem != null)
        {
            armour.RemoveModifier(oldItem.armourModifier);
            damage.RemoveModifier(oldItem.damageModifier);
            damageText.text = "Damage: " + this.damage.GetValue();
            armourText.text = "Armour: " + this.armour.GetValue();
        }
    }

    public override void takeDamage(int damage)
    {
        base.takeDamage(damage);
        AudioManager.instance.Play("PlayerHit");
        HealthText.text = this.currentHealth.ToString();
        healthFill.fillAmount = (float)currentHealth / (float)maxHealth;
        if (currentHealth <= 0)
        {
            Die();
        }
    }

    public override int HealthUp(int healthUp)
    {
        if (base.HealthUp(healthUp) == 1)
        {
            HealthText.text = this.currentHealth.ToString();
            healthFill.fillAmount = (float)currentHealth / (float)maxHealth;
            return 1;
        }
        return 0;
    }
    public override void Die()
    {
        base.Die();
        //player death sequence
        AudioManager.instance.Play("GameOver");
        PlayerManager.instance.killPlayer();
    }

}
