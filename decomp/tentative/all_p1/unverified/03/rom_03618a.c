/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03618a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3618a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    if (0 < (int)unaff_A6[0xd]) {
      unaff_A6[0xd] = 0;
    }
    unaff_A6[0xc] = 0;
    func_0x000143dc();
  }
  else {
    candidate_3663e();
    func_0x000143e4();
  }
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x9d) = *(char *)((int)unaff_A6 + 0x9d) + '\x01';
  if (0x4f < *(byte *)((int)unaff_A6 + 0x9d)) {
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
  if (*(char *)((int)unaff_A6 + 0x9d) == '(') {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  func_0x0003623e();
  return;
}
