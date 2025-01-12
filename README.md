# 🎯 **David Fitness**  
Proiect realizat pentru cursul de **Programare Orientată pe Obiecte** la **Universitatea din Craiova (UCV)**.

---

## 🏋️‍♂️ **Descrierea aplicației**  
**David Fitness** este o aplicație de fitness compatibilă cu **Linux** și **Windows** (în cazul Windows, este necesară compilarea specifică). Aplicația permite utilizatorului să își gestioneze antrenamentele și să monitorizeze progresul.

### 🔑 **Funcționalități principale**  
- ✅ Alegerea unui utilizator dintr-o listă existentă.  
- ✅ Crearea unui **cont nou**, dacă utilizatorul nu există deja.  
- ✅ Selecția unui **workout** dintr-o listă predefinită de antrenamente.  
- ✅ **Incrementarea exercițiilor** pentru a reflecta progresul în timp.  
- ✅ Sortarea exercițiilor:  
  - Alfabetic ➡️ apăsând tasta **"a"**  
  - După **caloriile arse** ➡️ tasta **"c"**  
  - După **progresul realizat** ➡️ tasta **"p"**  

### ➕ **Adăugarea unui nou workout**  
Utilizatorul poate adăuga un antrenament nou, introducând numele unui **workout disponibil**.

---

## 💾 **Persistență datelor**  
Aplicația salvează **utilizatorii** și **workout-urile lor** în fișiere JSON, asigurând persistența datelor.

---

## 🛠️ **Tehnologii utilizate**  
- Limbaj: **C++**  
- Biblioteci:  
  - 🖼️ **Raylib** – pentru interfața grafică  
  - 📄 **nlohmann/json** – pentru manipularea fișierelor JSON  
- Unelte pentru compilare și debugging:  
  - 🔧 **Make** – automatizarea procesului de compilare  
  - 🛠️ **Valgrind** – pentru detectarea erorilor de memorie  
  - 🐞 **GDB** – pentru depanare

---

## 📋 **Instrucțiuni de compilare**  
Aplicația poate fi compilată folosind **Makefile**. Asigurați-vă că aveți instalate toate librăriile necesare.

---

