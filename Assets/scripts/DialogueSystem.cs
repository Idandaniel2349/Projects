using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DialogueSystem : MonoBehaviour
{
    public static DialogueSystem instance { get; set; }
    public GameObject dialoguePanel;
    public string npcName;
    public List<string> dialogueLines = new List<string>();

    Button continueButton;
    Text dialogueText, nameText;
    int dialogueIndex;

    void Awake()
    {
        continueButton = dialoguePanel.transform.Find("Continue").GetComponent<Button>();// get the continue button object 
        dialogueText= dialoguePanel.transform.Find("Text").GetComponent<Text>();
        nameText = dialoguePanel.transform.Find("Name").Find("Text").GetComponent<Text>();


        continueButton.onClick.AddListener(delegate { continueDialogue(); });


        if (instance != null && instance != this) //if there is already a dialogue destroy it
        {
            Destroy(gameObject);
        }
        else//else set the dialogue to the new one
        {
            instance = this;
        }
    }

    public void AddNewDialogue(string[] lines,string npcName)
    {
        dialogueIndex = 0;
        dialogueLines= new List<string>(lines.Length);
        dialogueLines.AddRange(lines);
        this.npcName = npcName;
        createDialogue();
    }

    public void createDialogue()
    {
        dialogueText.text = dialogueLines[dialogueIndex];//set the dialogue text to the first element in the string list
        nameText.text = npcName;
        dialoguePanel.SetActive(true);
    }
    public void continueDialogue()
    {
        if (dialogueIndex < dialogueLines.Count-1)
        {
            dialogueIndex++;
            dialogueText.text = dialogueLines[dialogueIndex];
        }
        else
        {
            dialoguePanel.SetActive(false);
        }
    }
}
