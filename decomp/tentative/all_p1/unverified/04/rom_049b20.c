/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049b20.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49b20(void)

{
  int iVar1;
  char cVar2;
  byte bVar3;
  int unaff_A6;
  bool bVar4;

  *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -3;
  iVar1 = *(int *)(unaff_A6 + 0x30);
  if (iVar1 < -0x10000) {
    bVar3 = 0;
  }
  else if (iVar1 < -0xc000) {
    bVar3 = 1;
  }
  else if (iVar1 < -0x8000) {
    bVar3 = 2;
  }
  else if (iVar1 < -0x4000) {
    bVar3 = 3;
  }
  else if (iVar1 < 0x4000) {
    bVar3 = 4;
  }
  else if (iVar1 < 0x8000) {
    bVar3 = 5;
  }
  else if (iVar1 < 0xc000) {
    bVar3 = 6;
  }
  else if (iVar1 < 0x10000) {
    bVar3 = 7;
  }
  else {
    bVar3 = 8;
  }
  bVar4 = bVar3 < *(byte *)(unaff_A6 + 0x96);
  if (*(byte *)(unaff_A6 + 0x96) < bVar3) {
    if (*(char *)(unaff_A6 + 0x9b) == '\0') {
      bVar3 = *(char *)((short)(*(ushort *)(unaff_A6 + 0x80) >> 4) + 0xe09f4) - 3;
      if ((char)bVar3 < '\x04') {
        bVar4 = bVar3 < *(byte *)(unaff_A6 + 0x96);
        if ((char)bVar3 < (char)*(byte *)(unaff_A6 + 0x96)) {
          bVar3 = *(byte *)(unaff_A6 + 0x96);
          bVar4 = 0xfe < bVar3;
          *(byte *)(unaff_A6 + 0x96) = bVar3 + 1;
        }
      }
      else {
        bVar3 = *(byte *)(unaff_A6 + 0x96);
        bVar4 = 0xfe < bVar3;
        *(byte *)(unaff_A6 + 0x96) = bVar3 + 1;
      }
    }
    else {
      bVar3 = *(byte *)(unaff_A6 + 0x96);
      bVar4 = 0xfe < bVar3;
      *(byte *)(unaff_A6 + 0x96) = bVar3 + 1;
    }
  }
  if (bVar4) {
    if (*(char *)(unaff_A6 + 0x9b) == '\0') {
      cVar2 = *(char *)((short)(*(ushort *)(unaff_A6 + 0x80) >> 4) + 0xe09f4) + -3;
      if (cVar2 < '\x05') {
        *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
      }
      else if (*(char *)(unaff_A6 + 0x96) < cVar2) {
        *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
      }
    }
    else {
      *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
    }
  }
  if (*(char *)(unaff_A6 + 0x96) < '\x05') {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  return;
}
