/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5286.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5286(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfffe;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfffe;
  if (*(short *)(unaff_A6[3] + 0x70) == 0) {
    candidate_9ea8();
    candidate_4256();
  }
  else {
    candidate_9ea8();
    candidate_4256();
  }
  *unaff_A6 = 0xc52e4;
  if ((*(byte *)(unaff_A6[3] + 0x15) & 0x40) == 0) {
    return;
  }
  *unaff_A6 = 0xc52fa;
  candidate_12022();
  *(undefined2 *)(unaff_A6 + 0xf) = *(undefined2 *)(unaff_A6[3] + 0x3c);
  *(undefined2 *)(unaff_A6 + 0x10) = *(undefined2 *)(unaff_A6[3] + 0x40);
  bVar3 = false;
  candidate_427a();
  if (bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
