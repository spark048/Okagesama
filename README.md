# Okagesama
Interactive piece using FaceOSC in openFrameworks

CONCEPT

We cannot see ourselves without light, but if there is a light there is always our shadows as well. Duh. But I am implying psychologically as well as philosophically. 

When we see ourselves in light we also see our shadows. The shadows look too dark to bare sometimes. However shadows do not have their own colour. They look dark or soft depending on the strength of the light. Shadow only exists because there is light somewhere else.
The stronger light is the darker shadow becomes. Darkness of shadow is only defined by comparing it to brightness of light. If there is no light shadow is everywhere and every shadow merges into one. Not even one. No need to count it any more. There is no need to identify each. There is no each.  It is just IT. People tend to avoid shadow but we need to recognise that it is not necessary to do so as long as we know what it is.

Yayoi Kusama uses dots for self-obliteration. Wearing a dot patterned dress inside of the room which is surrounded by walls filled with dots, she obliterates herself. Which is the same act of connecting to the universe and it lets us recognise that there is no border between us and everything else.
In this piece I suggest an obliteration into shadow, an evacuation from the recognition of the world in which we have to reveal and show a lot of things to identify ourselves.

SYSTEM

In this interactive piece, which can be defined as a game, the audience (player) synchronises with the character called “Okagesama” (Mr. Shadow). He is shadow. “Okagesama” is a Japanese word to thank for someone’s backing or assistance. The kanji for "kage" means "shadow" and it was an expression for the gods and things around you that you cannot see. So it is not only used for person but also used to thank many things around you that bring you a fruitful moment. I designed this character long time ago and finally the time to put him on the stage has come. The player controls him to collect dark matter. If he gets a piece of dark matter he can spread his shadow into the area. Once he achieves covering enough space with his shadow you will see the message.

I used face, the symbol of identity, to control the shadow by the FaceOSC (github.com/kylemcdonald/ofxFaceTracker/releases) in openFrameworks (openframeworks.cc/).Okagesama moves right and left horizontally following the player’s face angle. If the player moves his/her eyebrows up the body length of Okagesama extends. If he has a dark matter the player can spread his shadow by opening his/her mouth. The wider the mouth opens the bigger the shadow gets. The more shadow gets spread the more darkness becomes mild.

FURTHER DEVELOPMENT

To calculate the occupation of the shadow I wrote a program which goes through all the pixels in the blank space and count the number of pixels that match the colour of shadow. But it still has some unstableness because of the movement of some other assets on the screen. At the moment I set threshold to correct it but there must be a better way to do so.
