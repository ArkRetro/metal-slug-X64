/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04dc5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_4dc5c(void)

{
  byte in_D0b;
  char cVar2;
  short sVar1;
  byte bVar3;
  int unaff_A6;
  bool bVar4;

  if ((*(byte *)(unaff_A6 + 0xa0) & 8) == 0) {
    return in_D0b;
  }
  cVar2 = candidate_aaa88();
  if (cVar2 == '\0') {
    if (*(char *)(unaff_A6 + 0xa4) == '\0') {
      sVar1 = candidate_260c8();
    }
    else {
      sVar1 = candidate_260c8();
    }
    if (7 < sVar1) {
      sVar1 = 0;
    }
    bVar3 = *(byte *)(sVar1 + 0x4dc54);
    if (bVar3 == 4) {
      bVar3 = 7;
    }
    if (bVar3 == 8) {
      bVar3 = 7;
    }
    bVar4 = bVar3 < 7;
    if (bVar3 == 7) {
      candidate_4dd86();
      if (bVar4) {
        bVar3 = 0;
      }
      else {
        bVar3 = 7;
        *(byte *)(unaff_A6 + 0xa0) = *(byte *)(unaff_A6 + 0xa0) | 0x40;
      }
    }
    else {
      *(byte *)(unaff_A6 + 0x9d) = bVar3;
      *(undefined1 *)(unaff_A6 + 0x9e) = 0xff;
    }
  }
  else {
    if (*(char *)(unaff_A6 + 0xa4) == '\0') {
      sVar1 = candidate_260c8();
    }
    else {
      sVar1 = candidate_260c8();
    }
    if (7 < sVar1) {
      sVar1 = 0;
    }
    bVar3 = *(byte *)(sVar1 + 0x4dc54);
    if (bVar3 == 4) {
      bVar3 = 7;
    }
    if (bVar3 == 8) {
      bVar3 = 7;
    }
    if (bVar3 == 7) {
      *(byte *)(unaff_A6 + 0xa0) = *(byte *)(unaff_A6 + 0xa0) | 0x40;
    }
    else {
      *(byte *)(unaff_A6 + 0x9d) = bVar3;
      *(undefined1 *)(unaff_A6 + 0x9e) = 0xff;
    }
  }
  return bVar3;
}
