/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042308.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42308(void)

{
  short sVar1;
  char cVar2;
  char cVar3;
  int unaff_A6;
  bool bVar4;

  *(undefined1 *)(unaff_A6 + 0x9f) = 0;
  bVar4 = false;
  candidate_1b04a();
  if (!bVar4) {
    sVar1 = candidate_423aa();
    candidate_4463e((char)*(undefined4 *)((short)(sVar1 * 4) + 0x44fca));
    cVar2 = candidate_260c8();
    if ((((cVar2 == '\x04') || (cVar2 == '\x05')) || (cVar2 == '\x06')) &&
       (((cVar3 = candidate_19dca(), cVar3 != '\0' || (cVar3 = candidate_19dca(), cVar3 != '\0')) ||
        (cVar3 = candidate_19dca(), cVar3 != '\0')))) {
      sVar1 = candidate_29c6();
      if (sVar1 < 0) {
        cVar2 = '\x03';
      }
      else {
        cVar2 = '\x02';
      }
    }
    *(char *)(unaff_A6 + 0x80) = cVar2;
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x80) = 0;
  return;
}
