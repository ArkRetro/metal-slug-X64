/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03c1d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3c1d6(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  unaff_A6[0xd] = (-0x100000 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 6);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x6e) - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    *(int *)((int)unaff_A6 + 0x6e) =
         (unaff_A6[0xc] - *(int *)((int)unaff_A6 + 0x6e) >> 6) + *(int *)((int)unaff_A6 + 0x6e);
  }
  func_0x000143e4();
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  *(short *)((int)unaff_A6 + 0x26) = (short)((uint)iVar2 >> 8);
  candidate_427a();
  if ((char)(*(byte *)(unaff_A6 + 0xc) ^ -((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0)) < '\0') {
    *unaff_A6 = candidate_3c296;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_3c296;
  }
  cVar3 = candidate_49e8();
  if (cVar3 != '\0') {
    *unaff_A6 = candidate_3c296;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_3c296;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
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
