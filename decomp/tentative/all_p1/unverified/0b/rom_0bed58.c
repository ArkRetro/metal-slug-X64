/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bed58.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bed58(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  unaff_A6[0x1c] = 0xbed34;
  *unaff_A6 = 0xbed6c;
  uVar3 = *(ushort *)unaff_A6[0x1c];
  if ((short)uVar3 < 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (uVar3 <= *(ushort *)(unaff_A5 + 0x699e)) {
    unaff_A6[0x1c] = (ushort *)unaff_A6[0x1c] + 2;
    *(ushort *)((int)unaff_A6 + 0x6e) = uVar3;
    candidate_9d1e();
    return;
  }
  func_0x00009cce();
  return;
}
