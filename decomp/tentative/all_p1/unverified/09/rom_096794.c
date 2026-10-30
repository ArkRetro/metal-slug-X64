/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096794.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96794(void)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  int unaff_A6;

  bVar1 = candidate_2eb4();
  bVar1 = bVar1 >> 2;
  if ((char)(bVar1 - 8) < '\t') {
    if (((byte)(bVar1 - 8) == '\b') && (*(int *)(unaff_A6 + 0x30) == 0)) {
      cVar2 = '\b';
    }
    else {
      cVar2 = bVar1 - 7;
    }
  }
  else {
    cVar2 = bVar1 - 7;
  }
  if ('\x0f' < cVar2) {
    cVar2 = '\x10';
  }
  if (cVar2 < '\0') {
    cVar2 = '\0';
  }
  if (*(char *)(unaff_A6 + 0x91) == '\0') {
    if (*(int *)(unaff_A6 + 0x34) < 0) {
      if (*(int *)(unaff_A6 + 0x30) < 0) {
        cVar2 = '\a';
      }
      else if (*(int *)(unaff_A6 + 0x30) < 1) {
        cVar2 = '\b';
      }
      else {
        cVar2 = '\t';
      }
    }
  }
  else if ((*(byte *)(unaff_A6 + 0x85) & 4) == 0) {
    if ((*(byte *)(unaff_A6 + 0x85) & 8) == 0) {
      if (*(int *)(unaff_A6 + 0x30) < 0) {
        cVar2 = '\a';
      }
      else if (*(int *)(unaff_A6 + 0x30) < 1) {
        cVar2 = '\b';
      }
      else {
        cVar2 = '\t';
      }
    }
    else {
      cVar2 = '\x10';
    }
  }
  else {
    cVar2 = '\0';
  }
  cVar3 = *(char *)(unaff_A6 + 0x86);
  if (cVar2 < cVar3) {
    cVar3 = cVar3 + -1;
  }
  else if (cVar3 < cVar2) {
    cVar3 = cVar3 + '\x01';
  }
  *(char *)(unaff_A6 + 0x86) = cVar3;
  return;
}
