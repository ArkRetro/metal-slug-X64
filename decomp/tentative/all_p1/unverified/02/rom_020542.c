/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020542.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20542(void)

{
  int iVar1;
  short sVar2;
  char cVar3;
  short sVar4;
  int unaff_A6;

  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0x82);
  if (iVar1 == 0) {
    *(undefined1 *)(unaff_A6 + 0x60) = 0;
    *(undefined2 *)(unaff_A6 + 0x86) = 0x7fff;
    if ((*(byte *)(unaff_A6 + 0x8c) & 8) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    return;
  }
  sVar2 = *(short *)(iVar1 + 0x2c) - *(short *)(unaff_A6 + 0x2c);
  *(short *)(unaff_A6 + 0x86) = sVar2;
  sVar4 = 0;
  do {
    if (sVar2 <= *(short *)(sVar4 + 0xdfe02)) {
      cVar3 = (char)*(undefined2 *)(sVar4 + 0xdfe04);
      if (cVar3 != *(char *)(unaff_A6 + 0x60)) {
        *(char *)(unaff_A6 + 0x60) = cVar3;
        if ((*(byte *)(unaff_A6 + 0x8c) & 8) == 0) {
          candidate_4256();
        }
        else {
          candidate_4256();
        }
      }
      return;
    }
    sVar4 = sVar4 + 4;
  } while (sVar4 < 0x18);
  return;
}
