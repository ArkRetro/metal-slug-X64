/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03bf1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3bf1c(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_3bf86;
  }
  unaff_A6[0xd] = (-0x100000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    if (*(char *)(unaff_A6 + 5) == *(char *)((int)unaff_A6 + 0x19)) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
  }
  bVar3 = false;
  candidate_19504();
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
