/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x031f52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_31f52(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0x14] = 0x31d54;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1ad6e();
  candidate_146e8();
  candidate_4256();
  *unaff_A6 = 0x31fbe;
  uVar2 = 0;
  candidate_320d0();
  candidate_320f8();
  iVar1 = unaff_A6[3];
  candidate_320ee();
  if ((bool)uVar2) {
    if (*(char *)(iVar1 + 0x80) == '\x01') {
      *unaff_A6 = candidate_32004;
    }
  }
  else if (*(char *)(iVar1 + 0x80) == '\x02') {
    *unaff_A6 = candidate_32004;
  }
  candidate_1436a();
  candidate_427a();
  return;
}
