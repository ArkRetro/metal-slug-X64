/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x067176.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_67176(void)

{
  int iVar1;
  undefined1 uVar2;
  char cVar3;
  byte bVar4;
  int *unaff_A3;
  int unaff_A5;

  iVar1 = *(int *)(unaff_A5 + 0x4586);
  uVar2 = *(undefined1 *)(iVar1 + 7);
  do {
    cVar3 = *(char *)(iVar1 + 7);
    if ((*(byte *)(iVar1 + 0xf) & 2) == 0) {
      bVar4 = cVar3 + 1;
      if (*(byte *)(iVar1 + 6) <= bVar4) {
        bVar4 = 0;
      }
    }
    else {
      bVar4 = cVar3 - 1;
      if (cVar3 == '\0') {
        bVar4 = *(char *)(iVar1 + 6) + bVar4;
      }
    }
    *(byte *)(iVar1 + 7) = bVar4;
    *(byte *)(iVar1 + 8) = bVar4;
    candidate_66a94();
  } while ((unaff_A3[1] == 0) || (*unaff_A3 == -2));
  *(undefined1 *)(iVar1 + 8) = uVar2;
  candidate_66ca4();
  *(undefined1 *)(iVar1 + 8) = *(undefined1 *)(iVar1 + 7);
  candidate_66ca4();
  *(undefined1 *)(iVar1 + 0xe) = 0;
  return;
}
