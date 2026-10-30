/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b4df8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b4df8(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_40bc();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *unaff_A6 = 0xb4e38;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x10000;
  func_0x000143e4();
  if (0x41 < *(ushort *)(unaff_A6 + 0xb)) {
    *unaff_A6 = candidate_b4e62;
  }
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xf) < 0) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_140c0();
  return;
}
