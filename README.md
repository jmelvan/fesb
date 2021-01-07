### FESB
# Strukture podataka

## Prakticni rad

Ideja: **Pracenje prisustva radnika na poslu**

### Realizacija:

Prilikom ucitavanja, program ucita zapis uredaja za timbravanje. Taj zapis spremljen je u datoteku `devicelog.txt`.

Primjer zapisa pohranjen je na sljedeci nacin:
```json
3842 1598946910
4320 1598947052
2635 1598947162
4320 1598975994
3842 1598976021
2635 1598976613
```
> Prvi broj oznacava ID pojedinog radnika, a drugi broj je timestamp vremena kada se radnik timbrao

Logika ucitavanja zapisa je sljedeca:
  1. Ucitava se linija po linija i pojedini timestamp se sprema pripadnom korisniku
  2. Jedan radni dan se smatra kao 2 timbravanja. Dakle prvo timbravanje je dolazak na posao, a drugo odlazak.
  3. Zanemaruju se pogreske ako se radnik zaboravio timbrati kada je odlazio sa posla jer bi takve pogreske handlao uredaj. Dakle, podrazumijeva se da se radnik timbrao 2 puta u jednom danu.

Nakon ucitavanja, korisniku se pojavi meni u kojem moze birati navedene opcije medu kojima se nalazi i opcija ispisivanja radnih sati za pojedinog radnika u unesenom vremenskom razdoblju.
Npr. Od **25.2.2020** do **2.3.2020**, gdje ce za odradene radne dane prikazati timeline, a za dane u kojima se radnik nije timbrao pisat da se radnik taj dan nije pojavio na poslu.

Osim te opcije postoji opcija da se export-a izvjestaj koji sadrzi razliku u satima izmedu nuznih i odradenih sati. Primjer izvjestaja:
```
Name      Surname        Total Hours         Total Minutes       Difference          

Duje      Senta            8 hrs              10 mins              +3.17 hrs
Jakov     Melvan          17 hrs              48 mins             +12.82 hrs
Mario     Jelic            0 hrs               0 mins              -5.00 hrs
Ante      Kuic             8 hrs               2 mins              +3.03 hrs
```
