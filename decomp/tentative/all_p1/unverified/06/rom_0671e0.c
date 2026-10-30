/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0671e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_671e0(void)

{
  int iVar1;
  char cVar2;
  undefined1 uVar3;
  byte in_D0b;
  int *unaff_A3;
  int unaff_A5;

  iVar1 = *(int *)(unaff_A5 + 0x4586);
  uVar3 = *(undefined1 *)(iVar1 + 7);
  if (*(byte *)(iVar1 + 6) <= in_D0b) {
    in_D0b = *(char *)(iVar1 + 6) - 1;
  }
  *(byte *)(iVar1 + 7) = in_D0b;
  while( true ) {
    *(byte *)(iVar1 + 8) = in_D0b;
    candidate_66a94();
    if ((unaff_A3[1] != 0) && (*unaff_A3 != -2)) break;
    cVar2 = *(char *)(iVar1 + 7);
    if ((*(byte *)(iVar1 + 0xf) & 2) == 0) {
      in_D0b = cVar2 + 1;
      if (*(byte *)(iVar1 + 6) <= in_D0b) {
        in_D0b = 0;
      }
    }
    else {
      in_D0b = cVar2 - 1;
      if (cVar2 == '\0') {
        in_D0b = *(char *)(iVar1 + 6) + in_D0b;
      }
    }
    *(byte *)(iVar1 + 7) = in_D0b;
  }
  *(undefined1 *)(iVar1 + 8) = uVar3;
  candidate_66ca4();
  *(undefined1 *)(iVar1 + 8) = *(undefined1 *)(iVar1 + 7);
  candidate_66ca4();
  *(undefined1 *)(iVar1 + 0xe) = 0;
  return;
}
