��    E      D  a   l      �  [   �     M  (   a     �     �  	   �     �     �     �  (   �          5     U     k  ,   t     �  :   �  1   �  9   *     d  %   v  #   �  +   �  +   �  1   	  1   J	  %   |	  +   �	  1   �	  1    
  /   2
     b
     z
     �
     �
  .   �
  +   �
          8  !   S  5   u  "   �  0   �     �          /  )   L     v     �  %   �  &   �     �       2   #  2   V  0   �  -   �  	   �     �     �     	          (     @     [     p  "   �     �  &  �  \   �     C  %   ^     �     �     �     �     �     �  ,   �       0   2     c     u  -   ~     �  :   �  1     9   6     p  "   �  #   �  0   �  -   �  /   &  ,   V     �  ,   �  2   �  /     B   1     t      �  1   �     �  K     L   N  0   �     �  /   �  O     +   b  8   �     �  (   �       :   !  +   \     �  #   �  &   �  !   �       A   )  A   k  9   �  1   �  
        $     +     9     H     V     n     �     �      �     �     
      =   ,   )      ?       1       .      3              :       	   -         >           2           8                         D      !   9   $       A   "          0   #   /             %                        E                       *                7      <   @   4      ;           &   C   '           5      6      (      B           +           
The following ARM specific disassembler options are supported for use with
the -M switch:
 # <dis error: %08x> # internal error, undefined modifier(%c) $<undefined> %02x		*unknown* *unknown* <function code %d> <illegal precision> <internal disassembler error> <internal error in opcode table: %s %s>
 <unknown register %d> Address 0x%x is out of bounds.
 Don't understand %x 
 Hmmmm %x Illegal limm reference in last instruction!
 Internal disassembler error Internal error:  bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Internal error: bad sparc-opcode.h: "%s" == "%s"
 Internal error: bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Unknown error %d
 Unrecognised disassembler option: %s
 Unrecognised register name set: %s
 Unrecognized field %d while building insn.
 Unrecognized field %d while decoding insn.
 Unrecognized field %d while getting int operand.
 Unrecognized field %d while getting vma operand.
 Unrecognized field %d while parsing.
 Unrecognized field %d while printing insn.
 Unrecognized field %d while setting int operand.
 Unrecognized field %d while setting vma operand.
 attempt to set y bit when using + or - modifier bad instruction `%.50s' bad instruction `%.50s...' branch operand unaligned branch to odd offset branch value not in range and to an odd offset branch value not in range and to odd offset branch value out of range can't cope with insert %d
 displacement value is not aligned displacement value is not in range and is not aligned displacement value is out of range ignoring least significant bits in branch offset illegal bitmask immediate value is out of range immediate value must be even immediate value not in range and not even index register in load range invalid conditional option invalid register for stack adjustment invalid register operand when updating jump hint unaligned junk at end of line operand out of range (%ld not between %ld and %ld) operand out of range (%lu not between %lu and %lu) operand out of range (%lu not between 0 and %lu) syntax error (expected char `%c', found `%c') undefined unknown unknown	0x%02x unknown	0x%04lx unknown	0x%04x unknown constraint `%c' unknown operand shift: %x
 unknown pop reg: %d
 unrecognized instruction unrecognized keyword/register name value out of range Project-Id-Version: opcodes 2.11
POT-Creation-Date: 2001-01-11 12:44-0800
PO-Revision-Date: 2001-10-23 17:35+0200
Last-Translator: Keld Simonsen <keld@dkuug.dk>
Language-Team: Danish <dansk@klid.dk>
MIME-Version: 1.0
Content-Type: text/plain; charset=iso-8859-1
Content-Transfer-Encoding: 8bit
 
F�lgende ARM-specifikke disassembleralternativ underst�ttes for brug
sammen med flaget -M:
 # <disassemblerfejl: %08x> # intern fejl, ukendt modifikator(%c) $<udefineret> %02x		*ukendt* *ukendt* <funktionskode %d> <ugyldig pr�cision> <intern fejl i disassembleren> <intern fejl i instruktionstabellen: %s %s>
 <ukendt register %d> Adressen 0x%x ligger uden for tilladte gr�nser.
 Forst�r ikke %x 
 Hmmmm %x Ugyldig limm-reference i sidste instruktion!
 Intern fejl i disassembleren Intern fejl:  d�rlig sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Intern fejl: d�rlig sparc-opcode.h: "%s" == "%s"
 Intern fejl: d�rlig sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Ukendt fejl %d
 Ukendt disassembleralternativ: %s
 Ukendt registernavn er angivet: %s
 Ukendt felt %d ved konstruktion af instruktion.
 Ukendt felt %d ved afkodning af instruktion.
 Ukendt felt %d ved hentning af heltalsoperand.
 Ukendt felt %d ved hentning af vma-operand.
 Ukendt felt %d ved tolkning.
 Ukendt felt %d ved udskrift af instruktion.
 Ukendt felt %d ved indstilling af heltalsoperand.
 Ukendt felt %d ved indstilling af vma-operand.
 fors�g p� at s�tte y-bitten n�r modifikatoren + eller - blev brugt fejlagtig instruktion "%.50s" fejlagtig instruktion "%.50s..." operanden for betinget hop ligger p� sk�v adresse betinget hop til ulige afs�t v�rdien for betinget hop er ikke indenfor intervallet og til et ulige afs�t v�rdien for betinget hop er ikke inden for intervallet og til et ulige afs�t v�rdien for betinget hop er uden for intervallet kan ikke inds�tte %d
 forskydningsv�rdien ligger ikke p� lige adresse forskydningsv�rdien er ikke indenfor intervallet og ligger ikke p� lige adresse forskydningsv�rdien er uden for intervallet ignorerer mindste betydende bit i afs�t for betinget hop ugyldig bitmaske umiddelbar v�rdi er uden for intervallet umiddelbar v�rdi skal v�re lige umiddelbar v�rdi er ikke indenfor intervallet og ikke lige indeksregistret er i indl�sningsintervallet ugyldigt betinget flag ugyldigt register for stakjustering ugyldig registeroperand ved opdatering hopper�det ligger p� sk�v adresse snavs ved slutning p� linjen operanden er uden for intervallet (%ld er ikke mellem %ld og %ld) operanden er uden for intervallet (%lu er ikke mellem %lu og %lu) operanden uden for intervallet (%lu ikke mellem 0 og %lu) syntaksfejl (tegnet "%c" forventedes, fandt "%c") udefineret ukendt ukendt	0x%02x ukendt	0x%04lx ukendt	0x%04x ukendt begr�nsning "%c" ukendt operandskiftning: %x
 ukendt pop-register: %d
 ukendt instruktion ukendt navn p� n�gleord/register v�rdien er uden for intervallet 