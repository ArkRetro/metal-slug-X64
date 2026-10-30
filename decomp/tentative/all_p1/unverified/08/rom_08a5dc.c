/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a5dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a5dc(void)

{
  code *pcVar1;
  short in_D0w;
  int iVar2;
  int in_A0;
  int *unaff_A6;

  pcVar1 = *(code **)(in_A0 + (short)(in_D0w * 4));
  if ((pcVar1 != candidate_89bfe) && (pcVar1 != (code *)0x89c12)) {
    *unaff_A6 = (int)pcVar1;
    return;
  }
  iVar2 = candidate_8a5d4();
  if (iVar2 != 0) {
    *unaff_A6 = (int)pcVar1;
  }
  return;
}
